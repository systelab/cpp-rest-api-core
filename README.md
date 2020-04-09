[![Build Status](https://travis-ci.org/systelab/cpp-rest-api-core.svg?branch=master)](https://travis-ci.org/systelab/cpp-rest-api-core)
[![Build status](https://ci.appveyor.com/api/projects/status/ix9b122obt2f2j9v?svg=true)](https://ci.appveyor.com/project/systelab/cpp-rest-api-core)
[![codecov](https://codecov.io/gh/systelab/cpp-rest-api-core/branch/master/graph/badge.svg)](https://codecov.io/gh/systelab/cpp-rest-api-core)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/8e7fca4ee7cb47a1801f51771532b2cb)](https://www.codacy.com/app/systelab/cpp-rest-api-core?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=systelab/cpp-rest-api-core&amp;utm_campaign=Badge_Grade)
[![Download](https://api.bintray.com/packages/systelab/conan/RESTAPICore:systelab/images/download.svg)](https://bintray.com/systelab/conan/RESTAPICore:systelab/_latestVersion)


# C++ REST API framework

This repository implements a C++ framework to facilitate the creation of REST API.

## Supported features

* Routing mechanism
* Authorization through JWT
* User access validation
* Token expiration validation
* Custom access validation

## Setup

### Build from sources

Prerequisites:
  - [Git](https://git-scm.com/)
  - [Conan](https://conan.io/)
  - [CMake](https://cmake.org/)
  - [Visual Studio 2019](https://visualstudio.microsoft.com/) (only on Windows)
  - [GCC](https://gcc.gnu.org/) (only on Linux)

Build library with the following steps:
  1. Clone this repository in a local drive
  2. Make a build directory (i.e. `build/`)
  3. Install `conan` dependencies in the build directory
  4. Run `cmake` in the build directory to configure build targets
  5. Use `Visual Studio` (on Windows) or `make` (on Linux) to build the library

#### Windows
``` bash
> git clone https://github.com/systelab/cpp-rest-api-core
> md build && cd build
> conan install .. -s build_type=Debug -s compiler.toolset=v142 -s arch=x86_64
> cmake .. -G "Visual Studio 16 2019" -A x64
> devenv.exe RESTAPICore.sln
```

#### Linux
``` bash
> git clone https://github.com/systelab/cpp-rest-api-core
> mkdir build && cd build
> conan install ..
> cmake .. -DCMAKE_BUILD_TYPE=[Debug | Coverage | Release]
> make
```

### Download using Conan

  1. Create/update your `conanfile.txt` to add this library as follows:

```
[requires]
RESTAPICore/1.0.0@systelab/stable

[generators]
cmake
```

  2. Integrate Conan into CMake by adding the following code into your `CMakeLists.txt`:

```cmake
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
```

  3. Link against `${CONAN_LIBS}` when configuring your executables in CMake:

```cmake
set(MY_PROJECT MyProject)
add_executable(${MY_PROJECT} main.cpp)
target_link_libraries(${MY_PROJECT} ${CONAN_LIBS})
```

## Usage

### Configure basic routing

1) Implement an endpoint by creating a class that inherits ```systelab::rest_api_core::IEndpoint``` interface:

```cpp
#include "RESTAPICore/Endpoint/IEndpoint.h"

class YourEndpoint : public systelab::rest_api_core::IEndpoint
{
public:
    YourEndpoint() = default;

    std::unique_ptr<systelab::web_server::Reply>
    execute(const systelab::rest_api_core::EndpointRequestData&) override
    {
        // Process given systelab::rest_api_core::EndpointRequestData
	// and generate a systelab::web_server::Reply
        return reply;
    }
};
```


2) Create a web service that sets up a router with a single route registered:

```cpp
#include "RESTAPICore/Router/Router.h"
#include "RESTAPICore/Router/RoutesFactory.h"

class RESTAPIWebService : public systelab::web_server::IWebService
{
public:
    RESTAPIWebService()
    {
        std::string jwtKey = "HereGoesYourJWTSecretKey";
	auto routesFactory = std::make_unique<systelab::rest_api_core::RoutesFactory>(jwtKey);
	
	m_router = std::make_unique<systelab::rest_api_core::Router>();
	m_router->addRoute(routesFactory.buildRoute("GET", "/rest/api/yourendpoint", {},
		                                    []() { return std::make_unique<YourEndpoint>() }) );
	// Register more routes here
    }

    std::unique_ptr<systelab::web_server::Reply>
    process(const systelab::web_server::Request& request) const override
    {
        return m_router->process(request);
    }

private:
    std::unique_ptr<systelab::rest_api_core::Router> m_router;
};
```

> Thus, when the web service receives a GET HTTP request with "/rest/api/yourendpoint" URI, it redirects this request to the ```YourEndpoint```class implemented previously.


3) Register additional routes to other endpoints:

```cpp
router->addRoute(routesFactory.buildRoute("POST", "/rest/api/yourendpoint", {},
                                          []() { return std::make_unique<YourPostEndpoint>() });
router->addRoute(routesFactory.buildRoute("PUT", "/rest/api/yourendpoint", {},
                                          []() { return std::make_unique<YourPutEndpoint>() });
router->addRoute(routesFactory.buildRoute("DELETE", "/rest/api/yourendpoint", {},
                                          []() { return std::make_unique<YourDeleteEndpoint>() });
router->addRoute(routesFactory.buildRoute("GET", "/rest/api/anotherendpoint", {},
                                          []() { return std::make_unique<AnotherGetEndpoint>() });
```


### Routes with parameters

Routes with parameters can be registered by using an specific syntax on associated URIs:

```cpp
// Route with an string parameter named 'id'
router->addRoute(routesFactory.buildRoute("GET", "/rest/api/yourendpoint/:id", {},
                                          []() { return std::make_unique<YourGetIdEndpoint>() });
					  
// Route with a numeric parameter named 'number'
router->addRoute(routesFactory.buildRoute("GET", "/rest/api/anotherendpoint/+number", {},
                                          []() { return std::make_unique<AnotherGetIdEndpoint>() });
	 
// Route with multiple parameters
router->addRoute(routesFactory.buildRoute("GET", "/rest/api/yourendpoint/+id1/:id2", {},
                                          []() { return std::make_unique<YourGetMultipleParamsEndpoint>() });
```

When a request matches a registered route with parameters, the associated endpoint is called with a `systelab::rest_api_core::EndpointRequestData` object that contains these parameters properly parsed for easy usage:

```cpp
std::unique_ptr<systelab::web_server::Reply>
YourGetMultipleParamsEndpoint::execute(const systelab::rest_api_core::EndpointRequestData& requestData)
{
    unsigned int id1 = requestData.getParameters().getNumericParameter("id1");
    std::string id2 = requestData.getParameters().getStringParameter("id2");
    ...
}
```


### Authorization through JWT

When working with a [Bearer Authentication](https://swagger.io/docs/specification/authentication/bearer-authentication/) scheme, the library automatically decodes the tokens contained on `Authorization` header of HTTP requests. It uses the key provided on the `RoutesFactory` constructor to verify that token signature is correct. Then, claims contained on decoded [JSON Web Tokens](https://jwt.io/) are provided to the matching endpoint as part of the `systelab::rest_api_core::EndpointRequestData` object:

```cpp
std::unique_ptr<systelab::web_server::Reply>
YourEndpoint::execute(const systelab::rest_api_core::EndpointRequestData& requestData)
{
    auto& authorizationClaims = requestData.getAuthorizationClaims();
    std::vector<std::string> claimNames = authorizationClaims.getClaimNames();
    for (auto name : claimNames)
    {
        std::string claimValue = authorizationClaims.getClaim(name);
	...
    }	
    ...
}
```


### Routes with user role access validation

Routes can be configured to allow access only to users that have a certain role. This can be achieved through *route access validators*, a middleware that is executed after matching a request with a route, but before dispatching it to the endpoint. So, user role validation can be set up as follows:

1) Create a class that implements `IUserRoleService` interface. The method `getUserRoles()` should return the names of the role(s) associated to the user with the given username according to application bussiness logic.

```cpp
#include "RESTAPICore/RouteAccess/IUserRoleService.h"

class YourUserRoleService : public systelab::rest_api_core::IUserRoleService
{
public:
    YourUserRoleService() = default;

    std::vector<std::string> getUserRoles(const std::string& username) override
    {
        ...
    }
};

```

2) Add an instance of the implemented user role service into the REST API web service:

```cpp
class RESTAPIWebService : public systelab::web_server::IWebService
{
public:
   ...
   
private:
    ...
    YourUserRoleService m_userRoleService;
};
```

3) Use the built-in `UserRoleRouteAccessValidator` to register routes that only allow access to users of a certain role:

```cpp
#include "RESTAPICore/RouteAccess/UserRoleRouteAccessValidator.h"

// Route that only allows access to 'Admin' users
m_router->addRoute(routesFactory.buildRoute("GET", "/rest/api/yourendpoint",
                   { [this](){ return std::make_unique<UserRoleRouteAccessValidator>({"Admin"}, m_userRoleService) } },
		   []() { return std::make_unique<YourEndpoint>() }) );

// Route that allows access to 'Admin' and 'Basic' users
m_router->addRoute(routesFactory.buildRoute("GET", "/rest/api/anotherendpoint",
                   { [this](){ return std::make_unique<UserRoleRouteAccessValidator>({"Admin", "Basic"}, m_userRoleService) } },
		   []() { return std::make_unique<AnotherEndpoint>() }) );
```

> When the web service receives an HTTP request that matches any of these routes, before redirecting the request to the endpoint, it will check if the request has an authorization claim for the subject ("sub"). If so, this claim will be used to retrieve the associated user roles and then, these roles will be compared against the allowed ones for the route. If user is allowed, then the request will be dispatched to the endpoint. Otherwise, a forbidden reply will be returned.


### Routes with token expiration validation

Another built-in route access validator (class `TokenExpirationAccessValidator`) can be used to check if the token contained on the `Authorization` header of the requests has expired or not. This verification is based on the "Issued at" (iat) authorization claim and the current time. Thus, when current time is greater than iat plus a configured expiration time (in seconds), a forbidden reply will be returned.  

```cpp
#include "RESTAPICore/RouteAccess/TokenExpirationAccessValidator.h"

// Route that does not allow using tokens generated more than 10 min (600 seconds) ago
m_router->addRoute(routesFactory.buildRoute("GET", "/rest/api/yourendpoint",
                   { [this](){ return std::make_unique<TokenExpirationAccessValidator>(600, m_epochTimeService) } },
		   []() { return std::make_unique<YourEndpoint>() }) );
```

> The `m_epochTimeService` member is an instance of the `EpochTimeService` class, which provides a method to query the current time of the system.


### Custom access validation

This library can be extended by implementing any other kind of route access validation through the `IRouteAccessValidator` interface. The `hasAccess()` method can make use of any data contained on the `EndpointRequestData` argument to determine if the request has access to the route or not and return a boolean accordingly.

```cpp
#include "RESTAPICore/RouteAccess/IRouteAccessValidator.h"

class MyCustomRouteAccessValidator : public systelab::rest_api_core::IRouteAccessValidator
{
    MyCustomRouteAccessValidator(... )  // Inject any object required

    bool hasAccess(EndpointRequestData&) const override
    {
        // Use EndpointRequestData or injected objects to return if request has access to the route
    }
};
```
