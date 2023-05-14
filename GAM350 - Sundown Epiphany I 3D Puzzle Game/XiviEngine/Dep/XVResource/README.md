# XVResource
This repository contains the resource management that I am using for my personal project in XVEngine.

## To Use
The library is dependent on XVUtils for the base type (u64,Ptr<T>,etc...) and the assertion (built-in with spdlog).
Hence, you will have to include XVUtils into your project.

To use the resource manager, you can defined the following specialisation:
```c++
using RM = Resource::Manager <Resource1, Resource2, Resource3>;
RM manager;
```

This resource manager is able to hold Resource1, Resource2, Resource3.
The resource manager is implemented through CRTP pattern.

To set-up a loader for a specific resource type:
```c++
class Resource1Loader : Resource::ILoader { // Codes };
manager.SetLoader<Resource1>(new Resource1Loader);
```

The above binds a resource loader to that specific resource type. 
Such that all get and load operation through the ResourceManager will access the Resource Loader to load and cache the resource into the manager.

To get resource type:
```c++
manager.Get<Resource1>(file);
manager.Get<Resource1, Resource1Specialisation>(file);
```

The resource manager is able to store and retrieved specialised resource type.

## To Do
- Add in resource mapping functionality and resource metadata to work alongside the manager.
- Add in proper documentation.