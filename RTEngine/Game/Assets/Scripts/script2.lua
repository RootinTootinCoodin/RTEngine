script = {}
--Do not touch
script["UUID"] = 0
--
script["number"] = 0
function script:Start()
    script["number"] = 1
end
function script:Update()
    BASIC.LUALog("Loggings")
end
