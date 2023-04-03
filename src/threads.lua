local threads = {}

local C = require 'libthreads'

function threads.async(handler, arguments)
    local async_result = C.LIB_async(handler, arguments)

    local result = {}
    result.async_result = async_result
    result.ready_pointer = async_result.ready_pointer
    result.get = function (self) return self.async_result.get(self.async_result.thread_pointer) end
    result.check = function (self) return self.async_result.check(self.async_result.ready_pointer) end
    return result
end

return threads