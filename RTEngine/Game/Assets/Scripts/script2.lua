script2 = {}
--Do not touch
script2["UUID"] = 0
--
script2["speed"] = 1
script2["rotation_speed"] = 90
function script2:Start()
    script2["number"] = 1
end
function script2:Update()
    if INPUT.GetKeyState(script2["UUID"],26) == 2 then
        OUTPUT.MoveForward(script2["UUID"],script2["speed"]*BASIC.LUAGetDT())
    elseif INPUT.GetKeyState(script2["UUID"],22) == 2 then
        OUTPUT.MoveForward(script2["UUID"],-script2["speed"]*BASIC.LUAGetDT())
    end

    if INPUT.GetKeyState(script2["UUID"],7) == 2 then
        OUTPUT.RotateAlongY(script2["UUID"],script2["rotation_speed"]*BASIC.LUAGetDT())
    elseif INPUT.GetKeyState(script2["UUID"],4) == 2 then
        OUTPUT.RotateAlongY(script2["UUID"],-script2["rotation_speed"]*BASIC.LUAGetDT())
    end

    if INPUT.GetButtonState(script2["UUID"],1) == 1 then
    
        BASIC.Instantiate(script2["UUID"],"Bullet.westmodel",true)

    end
end