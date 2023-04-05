
local threads = require 'src/threads'

function sleep(a)
    local sec = tonumber(os.clock() + a)
    while (os.clock() < sec) do
    end
end

local future = threads.async(
    function (arguments)
        print("[TEST] Lua test function started")
        sleep(2)
        return 5
    end,
    {}
)

print("[TEST] Wait ready")
local counter = 0
while future:ready() == false do
    counter = counter + 1
end
print(counter)
print(future:get())

