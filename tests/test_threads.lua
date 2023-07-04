local threads = require 'src/threads'

local function sleep(a)
    local sec = tonumber(os.clock() + a)
    while (os.clock() < sec) do
    end
end

local future = threads.async(
    function(arguments)
        print("[TEST] lua test function started")
        sleep(2)
        return { value = "test success" }
    end,
    {}
)

print("[TEST] get result from future, but too early:", future:get())

print("[TEST] wait ready")
local counter = 0
while future:ready() == false do
    counter = counter + 1
end

local thread_result = future:get()
print("[TEST] finish call result:", future:finish())

print("[TEST] counter value:", counter)
print("[TEST] result value:", thread_result.value)
