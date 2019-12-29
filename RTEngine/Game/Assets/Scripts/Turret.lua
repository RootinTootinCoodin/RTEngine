function Turret()
    local table = {}
--Do not touch
    table["UUID"] = 0
--
    table["mouse_x"] = 0
    table["prev_mouse_x"] = 0
    table["rotation_threshold"] = 5
    table["rotation_degrees_per_pixel"] = 0.3

    function table:Start()
        table["mouse_x"] = INPUT.GetMouseX()
        table["prev_mouse_x"] = INPUT.GetMouseX()
    end
    function table:Update()
        table["prev_mouse_x"] =  table["mouse_x"]
        table["mouse_x"] = INPUT.GetMouseX()
        diff = table["mouse_x"] - table["prev_mouse_x"]
        if diff > table["rotation_threshold"] or diff < -table["rotation_threshold"] then
            OUTPUT.RotateAlongY(table["UUID"],diff*table["rotation_degrees_per_pixel"])
        end

        if INPUT.GetButtonState(table["UUID"],1) == 1 then
             BASIC.Instantiate(table["UUID"],"Bullet.westmodel",true,0,5,0,true)
        end
    end
    return table
end