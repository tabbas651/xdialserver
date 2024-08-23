/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2019 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#ifndef __RT_OBJECT_CACHE_H__
#define __RT_OBJECT_CACHE_H__

#include <string>

#include <assert.h>
//#include <rtError.h>
//#include <rtObject.h>
#include <chrono>
#include <unordered_map>

enum AppCacheErrorCodes {
    OK,
    NOT_FOUND,
    DUPLICATE_ENTRY,
    NULL_ENTRY
};

class AppInfo
{
    public:
        std::string appName;
        std::string appId;
        std::string appState;
        std::string appError;

        AppInfo(const std::string& name, const std::string& id, const std::string& state, const std::string& error)
            : appName(name),
              appId(id),
              appState(state),
              appError(error),
              lastTouched(std::chrono::steady_clock::now()) {}
        std::chrono::steady_clock::time_point lastTouched;
};

class rtRemoteObjectCache
{
public:
  rtRemoteObjectCache(){ }

    AppInfo* findObject(const std::string& appName) const
    {
        auto it = objects.find(appName);
        if (it != objects.end())
        {
            return it->second;
        }
        return nullptr;
    }

    AppCacheErrorCodes insert(std::string id, AppInfo* entry)
    {
        AppCacheErrorCodes returnValue = AppCacheErrorCodes::NULL_ENTRY;
        if (nullptr != entry)
        {
            auto result = objects.emplace(id, entry);
            if (!result.second)
            {
                returnValue = AppCacheErrorCodes::DUPLICATE_ENTRY;
            }
            else
            {
                returnValue = AppCacheErrorCodes::OK;
            }
        }
        return returnValue;
    }

    AppCacheErrorCodes touch(const std::string& appname)
    {
        AppCacheErrorCodes returnValue = AppCacheErrorCodes::NOT_FOUND;
        auto it = objects.find(appname);
        if (it != objects.end())
        {
            it->second->lastTouched = std::chrono::steady_clock::now();
            returnValue = AppCacheErrorCodes::OK;
        }
        return returnValue;
    }

    AppCacheErrorCodes erase(const std::string& appname)
    {
        AppCacheErrorCodes returnValue = AppCacheErrorCodes::NOT_FOUND;
        auto it = objects.find(appname);
        if (it != objects.end())
        {
            delete it->second;
            objects.erase(it);
            returnValue = AppCacheErrorCodes::OK;
        }
        return returnValue;
    }
private:
    std::unordered_map<std::string, AppInfo*> objects;
};
#endif