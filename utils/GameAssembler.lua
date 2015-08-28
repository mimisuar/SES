function main(gamename, savename, romdata, gfxname, sndname)
	local file = io.open(savename, "w")
	
	local romdata = io.open(romdata):read("*all")
	local romsize = romdata:len()
	
	file:write("[meta]\n")
	file:write("name=" .. gamename .. "\n")
	file:write("romsize=" .. string.format("%X", romsize) .. "\n")
	file:write("[data]\n")
	file:write("rom=" .. romdata .. "\n")
end

main(...)