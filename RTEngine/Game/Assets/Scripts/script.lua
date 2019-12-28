script = {}
--Do not touch
script["UUID"] = 0
--
script["number"] = 0
function script:Start()
    script["number"] = 1
end
function script:Update()
    if INPUT.GetKeyState(script["UUID"],26) == 2 then
        OUTPUT.MoveX(script["UUID"],0.1)
    elseif INPUT.GetKeyState(script["UUID"],22) == 2 then
        OUTPUT.MoveX(script["UUID"],-0.1)
    end

    if INPUT.GetKeyState(script["UUID"],7) == 2 then
        OUTPUT.MoveZ(script["UUID"],-0.1)
    elseif INPUT.GetKeyState(script["UUID"],4) == 2 then
        OUTPUT.MoveZ(script["UUID"],0.1)
    end
end
