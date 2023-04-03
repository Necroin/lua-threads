
local threads = require 'threads'
local fiber = require 'fiber'

local promise = threads.execute(
    function (arguments)
        ...
    end,
    {}
)

while promise:ready() == false do
    fiber.yeld()
end

local result = promise:get()

result


local threads = require 'src/threads'

local result = threads.async(
    function (arguments)
        print("Lua test function")
        return 5
    end,
    {}
)

local counter = 0
while result:check() == false do
    counter = counter + 1
    --print(counter)
end
print(counter)
print(result:get())

