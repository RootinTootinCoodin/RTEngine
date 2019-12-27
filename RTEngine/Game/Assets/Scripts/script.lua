script = {}
script["number"] = 0
function script:Start()
    script["number"] = 1
end
function script:Update()
    script["number"] = script["number"] + 1
end
