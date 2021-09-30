#!/bin/bash

#usage
function usage
{
	echo "Usage: $0 -o REPO_OWNER -s REPO_SLUG -t TAG -c CI -n CONFIG_NAME -j JOB_ID -p TEST_PROJECTS"
	echo ""
	echo
	echo "  -o ARG   Owner of the associated GitHub repository"
	echo "  -s ARG   Slug of the associated GitHub repository"
	echo "  -t ARG   Tag name associated to current build"
	echo "  -c ARG   Continuous Integration system (AppVeyor or Travis)"
	echo "  -n ARG   Configuration name of the build"
	echo "  -j ARG   Identifier of the build job"
	echo "  -p ARG   Comma separated list of test projects"
}

function checkErrors
{
	if [ $? != 0 ]
	then
		echo 
		echo "An error occured while deploying artifacts."
		echo
		exit 1
	fi
}

function parseParameters
{
	REPO_OWNER=systelab
	REPO_SLUG=
	TAG_NAME=
	CI_NAME=
	CONFIG_NAME=
	JOB_ID=
	TEST_PROJECTS=

	while getopts "ho:s:t:c:n:j:p:" OPTION 
	do
		case $OPTION in
		h)
			usage
			exit
			;;
		o)
			REPO_OWNER=$OPTARG
			;;
		s)
			REPO_SLUG=$OPTARG
			;;
		t)
			TAG_NAME=$OPTARG
			;;
		c)
			CI_NAME=$OPTARG
			;;
		n)
			CONFIG_NAME=$OPTARG
			;;
		j)
			JOB_ID=$OPTARG
			;;
		p)
			TEST_PROJECTS=$OPTARG
			;;
		?)
			echo "Invalid parameter" 
			usage
			exit
			;;
		esac
	done
}

function findTestProjectConfiguration
{
	TEST_PROJECT_REPORT=""
	TEST_PROJECT_TYPE=""

	echo "Searching configuration for test project $TEST_PROJECT_NAME"

	SEARCH_EXPRESSION="[$TEST_PROJECT_NAME]"
	INSIDE_SECTION=0
	while read line; do
		if [[ $INSIDE_SECTION == 1 ]]
		then
			if [ "${line:0:1}" == "[" ]
			then
				INSIDE_SECTION=0
			else
				if [ "${line:0:7}" == "report=" ]
				then
					TEST_PROJECT_REPORT="${line:7}"
				fi

				if [ "${line:0:5}" == "type=" ]
				then
					TEST_PROJECT_TYPE="${line:5}"
				fi
			fi
		else
			if [ "$line" =  "$SEARCH_EXPRESSION" ]
			then
				echo "Found section for project $TEST_PROJECT_NAME at line $line"
				INSIDE_SECTION=1
			fi
		fi
	done < $CONFIG_FILE

	# Process test project report file found
	if [[ -z "$TEST_PROJECT_REPORT" ]]
	then
		echo "Test report file for $TEST_PROJECT_NAME not found, using default (build/bin/$TEST_PROJECT_NAME.xml)"
		TEST_PROJECT_REPORT="build/bin/$TEST_PROJECT_NAME.xml"
	else
		echo "Test report file: $TEST_PROJECT_REPORT"
	fi
	
	# Process test project type found
	if [[ -z "$TEST_PROJECT_TYPE" ]]
	then
		echo "Test project type for $TEST_PROJECT_NAME not found, using default (Unit Tests)"
		TEST_PROJECT_TYPE="Unit Tests"
	else
		echo "Test project type: $TEST_PROJECT_TYPE"
	fi
	
	echo ""
}

function queryGitHubReleaseInternalId
{
	echo "Querying for GitHub Release internal identifier..."
	GITHUB_RELEASE_URL="https://api.github.com/repos/$REPO_OWNER/$REPO_SLUG/releases/tags/$TAG_NAME"
	echo "URL: $GITHUB_RELEASE_URL"

	GITHUB_RELEASE_DATA_JSON=$(curl --silent "$GITHUB_RELEASE_URL" -H "Authorization: token $GITHUB_ACTION_DISPATCH_TOKEN")
	checkErrors
	echo "GitHub Release data performed successfully."

	GITHUB_RELEASE_INTERNAL_ID=$(echo $GITHUB_RELEASE_DATA_JSON | python3 -c "import sys, json; print(json.load(sys.stdin)['id'])")
	echo "Release internal identifier is $GITHUB_RELEASE_INTERNAL_ID"
	echo ""	
}

function uploadTestReportToGitHub
{
	if [[ -z "$GITHUB_RELEASE_INTERNAL_ID" ]]
	then
		queryGitHubReleaseInternalId
	fi

	echo "Uploading test report $TEST_PROJECT_REPORT as an asset of GitHub release...";
	
	GITHUB_ASSET_FILENAME="$TEST_PROJECT_NAME-$JOB_ID.xml"
	GITHUB_ASSET_UPLOAD_URL="https://uploads.github.com/repos/$REPO_OWNER/$REPO_SLUG/releases/$GITHUB_RELEASE_INTERNAL_ID/assets?name=$GITHUB_ASSET_FILENAME"
	GITHUB_ASSET_CONTENT_TYPE=$(file -b --mime-type $TEST_PROJECT_REPORT)
	echo "URL: $GITHUB_ASSET_UPLOAD_URL"
	echo "Content-Type: $GITHUB_ASSET_CONTENT_TYPE"

	GITHUB_ASSET_UPLOAD_RESPONSE=$(curl --silent -H "Authorization: token $GITHUB_ACTION_DISPATCH_TOKEN" -H "Content-Type: $GITHUB_ASSET_CONTENT_TYPE" --data-binary @$TEST_PROJECT_REPORT $GITHUB_ASSET_UPLOAD_URL)
	checkErrors
	echo "Report uploaded successfully."

	TEST_PROJECT_ASSET_URL=$(echo $GITHUB_ASSET_UPLOAD_RESPONSE | python3 -c "import sys, json; print(json.load(sys.stdin)['url'])")
	checkErrors
	echo "Report asset URL is $TEST_PROJECT_ASSET_URL"
	echo ""
}

function buildTestProjectsJSON
{
	CONFIG_FILE="ci/build_docs.cfg"
	GITHUB_RELEASE_INTERNAL_ID=""

	TEST_PROJECTS_JSON="["

	FIRST_TEST_PROJECT=1
	for TEST_PROJECT_NAME in $TEST_PROJECTS
	do
		if [[ $FIRST_TEST_PROJECT == 0 ]]
		then
			TEST_PROJECTS_JSON+= ", "
		fi
		FIRST_TEST_PROJECT=0

		findTestProjectConfiguration
		uploadTestReportToGitHub

		TEST_PROJECT_JSON="{"
		TEST_PROJECT_JSON+="\"url\":"
		TEST_PROJECT_JSON+="\"${TEST_PROJECT_ASSET_URL}\","
		TEST_PROJECT_JSON+="\"name\":"
		TEST_PROJECT_JSON+="\"$TEST_PROJECT_NAME\","
		TEST_PROJECT_JSON+="\"type\":"
		TEST_PROJECT_JSON+="\"$TEST_PROJECT_TYPE\""
		TEST_PROJECT_JSON+="}"

		TEST_PROJECTS_JSON+="$TEST_PROJECT_JSON"

	done #TEST_PROJECT_NAME

	TEST_PROJECTS_JSON+="]" 
}

function dispatchDocBuildsEvent
{
	if [[ -z $REPO_SLUG ]]
	then
		echo
		echo "Syntax error: no repository slug specified. Use -s."
		exit 0
	fi

	if [[ -z $TAG_NAME ]]
	then
		echo
		echo "Syntax error: no tag name specified. Use -t."
		exit 0
	fi

	TEST_PROJECTS=${TEST_PROJECTS//,/ }

	echo "Dispatch GitHub Action to build documentation..."
	echo "REPO_OWNER=$REPO_OWNER"
	echo "REPO_SLUG=$REPO_SLUG"
	echo "TAG=$TAG_NAME"
	echo "CI=$CI_NAME"
	echo "CONFIG_NAME=$CONFIG_NAME"
	echo "JOB=$JOB_ID"
	echo "TEST_PROJECTS=$TEST_PROJECTS"
	echo ""
	
	buildTestProjectsJSON
	BODY_CONTENT="{\"event_type\": \"doc-build\", \"client_payload\": {\"tag\": \"$TAG_NAME\", \"configuration\": \"$CONFIG_NAME\", \"ci\": \"$CI_NAME\", \"job\": \"$JOB_ID\", \"tests\": $TEST_PROJECTS_JSON}}"
	echo "BODY_CONTENT is $BODY_CONTENT"
	echo ""

	curl -H "Authorization: token $GITHUB_ACTION_DISPATCH_TOKEN" \
		 -H 'Accept: application/vnd.github.everest-preview+json' \
		 "https://api.github.com/repos/$REPO_OWNER/$REPO_SLUG/dispatches" \
		 -d "$BODY_CONTENT"
	checkErrors
	echo "Done."
	echo ""
}


# MAIN
parseParameters "${@}"
dispatchDocBuildsEvent
