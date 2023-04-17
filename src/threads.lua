local threads = {}

local C = require 'libthreads'

local function deep_copy(argument)
    if type(argument) ~= "table" then return argument end
    local result = setmetatable({}, getmetatable(argument))
    for k, v in pairs(argument) do result[deep_copy(k)] = deep_copy(v) end
    return result
end

function threads.async(handler, arguments)
    local c_async_table = C.LIB_async(handler, arguments)

    local future = {}
    future.__c_async_table = c_async_table
    future.get = function (self)
        local result =  deep_copy(self.__c_async_table.get(self.__c_async_table.worker))
        return result
    end
    future.ready = function (self) return self.__c_async_table.check(self.__c_async_table.worker) end
    future.finish = function (self) return self.__c_async_table.finish(self.__c_async_table.worker) end
    return future
end

return threads