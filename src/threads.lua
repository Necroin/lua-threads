local threads = {}

local C = require 'libthreads'

function threads.async(handler, arguments)
    local c_async_table = C.LIB_async(handler, arguments)

    local future = {}
    future.__c_async_table = c_async_table
    future.get = function (self) return self.__c_async_table.get(self.__c_async_table.worker) end
    future.ready = function (self) return self.__c_async_table.check(self.__c_async_table.worker) end
    return future
end

return threads