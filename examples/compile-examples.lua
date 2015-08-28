_MAIN = "me"

package.path = "../utils/compiler/?.lua"
--package.cpath = "../"

require("compiler")

function main(example)
	example = tonumber(example)
	if example == 1 then
		-- Hello, world --
		compiler("helloworld.asm", "../Debug/helloworld")
		print("Compiled 'Hello, world.'.")
	elseif example == 2 then
		-- Hello, world (easy) --
		compiler("helloworld-easy.asm", "../Debug/helloworld-easy")
		print("Compiled 'Hello, world. Easy.'.")
	elseif example == 3 then
		-- Hello, world (loop) --
		compiler("helloworld-loop.asm", "../Debug/helloworld-loop")
		print("Compiled 'Hello, world. Loop.'")
	else
		print("compile-examples.lua <examplenumber>")
		print("Examples:")
		print("1:", "Hello, world.")
		print("2:", "Hello, world. Easy.")
		print("3:", "Hello, world. Loop.")
	end
end

main(...)