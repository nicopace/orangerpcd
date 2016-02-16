#!/usr/bin/lua

local juci = require("juci/core"); 

function sysupgrade_check(params)
	local type = {"usb", "online"}; 
	if params["type"] == "online" then type = {"online"}; 
	elseif params["type"] == "usb" then type = {"usb"} end;
	local res = { all = {} }; 
	for k,v in ipairs(type) do
		local path,ret = juci.shell("sysupgrade --"..v.." 2>/dev/null");  
		if( ret == 0 ) then
			path = path:gsub("\n", ""); 
			if path and path:len() > 0 then 
				res[v] = path;
				table.insert(res.all, path); 
			end
		end
	end
	return res; 
end

function sysupgrade_test(params)
	local res = {}; 
	local path = "/tmp/firmware.bin"; 
	if params["path"] then path = params["path"] end
	local ret,stdout,stderr = juci.exec("sysupgrade", { "--test", path }); 
	res["stdout"] = stdout;
	res["stderr"] = stderr; 
	res["error"] = ret; 
	return res; 
end

function sysupgrade_start(params)
	local res = {}; 
	local path = "/tmp/firmware.bin"; 
	local keep = ""; 
	if params["keep"] ~= 1 then keep = "-n" end
	if params["path"] then path = params["path"] end
	res["stdout"],res["error"] = juci.shell("sysupgrade "..keep.." %s", path); 
	return res; 
end

function sysupgrade_clean()
	local res = {}; 
	res["stdout"] = juci.shell("sysupgrade --clean"); 
	return res; 
end

function sysupgrade_online(params)

end

return {
	["check"] = sysupgrade_check, 
	["test"] = sysupgrade_test, 
	["start"] = sysupgrade_start,
	["clean"] = sysupgrade_clean,
	["online"] = sysupgrade_online
}; 

