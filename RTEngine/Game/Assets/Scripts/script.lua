script = {}
--Do not touch
script["UUID"] = 0
--
script["speed"] = 1
script["rotation_speed"] = 90
function script:Start()
    script["number"] = 1
end
function script:Update()
    if INPUT.GetKeyState(script["UUID"],26) == 2 then
        BASIC.Instantiate(script["UUID"],"Bullet.westmodel")
    elseif INPUT.GetKeyState(script["UUID"],22) == 2 then
        OUTPUT.MoveForward(script["UUID"],-script["speed"]*BASIC.LUAGetDT())
    end

    if INPUT.GetKeyState(script["UUID"],7) == 2 then
        OUTPUT.RotateAlongY(script["UUID"],script["rotation_speed"]*BASIC.LUAGetDT())
    elseif INPUT.GetKeyState(script["UUID"],4) == 2 then
        OUTPUT.RotateAlongY(script["UUID"],-script["rotation_speed"]*BASIC.LUAGetDT())
    end
end
