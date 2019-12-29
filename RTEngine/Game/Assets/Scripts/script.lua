function script()
local table = {}
--Do not touch
table["UUID"] = 0
--
table["speed"] = 1
table["rotation_speed"] = 90
function table:Start()
    table["number"] = 1
end
function table:Update()
    if INPUT.GetKeyState(table["UUID"],26) == 2 then
        OUTPUT.MoveForward(table["UUID"],table["speed"]*BASIC.LUAGetDT())
    elseif INPUT.GetKeyState(table["UUID"],22) == 2 then
        OUTPUT.MoveForward(table["UUID"],-table["speed"]*BASIC.LUAGetDT())
    end

    if INPUT.GetKeyState(table["UUID"],7) == 2 then
        OUTPUT.RotateAlongY(table["UUID"],table["rotation_speed"]*BASIC.LUAGetDT())
    elseif INPUT.GetKeyState(table["UUID"],4) == 2 then
        OUTPUT.RotateAlongY(table["UUID"],-table["rotation_speed"]*BASIC.LUAGetDT())
    end

    if INPUT.GetButtonState(table["UUID"],1) == 1 then
    
        BASIC.Instantiate(table["UUID"],"Bullet.westmodel",true)

    end
end
return table
end

-- table = {}
-- --Do not touch
-- table["UUID"] = 0
-- --
-- table["speed"] = 1
-- table["rotation_speed"] = 90
-- function table:Start()
--     table["number"] = 1
-- end
-- function table:Update()
--     if INPUT.GetKeyState(table["UUID"],26) == 2 then
--         OUTPUT.MoveForward(table["UUID"],table["speed"]*BASIC.LUAGetDT())
--     elseif INPUT.GetKeyState(table["UUID"],22) == 2 then
--         OUTPUT.MoveForward(table["UUID"],-table["speed"]*BASIC.LUAGetDT())
--     end

--     if INPUT.GetKeyState(table["UUID"],7) == 2 then
--         OUTPUT.RotateAlongY(table["UUID"],table["rotation_speed"]*BASIC.LUAGetDT())
--     elseif INPUT.GetKeyState(table["UUID"],4) == 2 then
--         OUTPUT.RotateAlongY(table["UUID"],-table["rotation_speed"]*BASIC.LUAGetDT())
--     end

--     if INPUT.GetButtonState(table["UUID"],1) == 1 then
    
--         BASIC.Instantiate(table["UUID"],"Bullet.westmodel",true)

--     end
-- end