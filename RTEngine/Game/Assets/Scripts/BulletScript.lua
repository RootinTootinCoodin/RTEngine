function BulletScript()
local table = {}
--Do not touch
table["UUID"] = 0
--
table["speed"] = 30
table["current_time"] = 0
table["bullet_time"] = 5

function table:Start()

end
function table:Update()
   table["current_time"] = table["current_time"]+BASIC.LUAGetDT()
   if table["current_time"] > table["bullet_time"] then
        BASIC.DeleteThis(table["UUID"])
   end

   OUTPUT.MoveForward(table["UUID"],table["speed"]*BASIC.LUAGetDT())
end
return table
end