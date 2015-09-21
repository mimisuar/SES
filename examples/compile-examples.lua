_MAIN = "me"

package.path = package.path .. ";../utils/compiler/?.lua"
package.path = package.path .. ";../utils/?.lua"

require("compiler")

examples = {
	{
		name="Hello World.",
		romfile="helloworld.sasm",
		romname="helloworld.ses"
	},
	{
		name="Graphics.",
		romfile="graphics.sasm",
		romname="graphics.ses"
	}
}

function main(example)
	example = tonumber(example)
	
	if examples[example] then
		print("Compiling " .. examples[example].name)
		compiler(examples[example].romfile, "../Debug/" .. examples[example].romname)
		return
	end
	
	print("compile-examples.lua <examplenumber>")
	print("Examples:")
	for i=1, #examples do
		print(i .. ":", examples[i].name)
	end
end

main(...)