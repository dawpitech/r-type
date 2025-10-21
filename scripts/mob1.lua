print("TRACE: Lua: Loading enemy behavior script.")

---[[
-- Define the system logic
local function enemy_ai_system(entity)
    -- Access components passed from C++
    local transform = entity.transform
    local velocity = entity.velocity

    print("TRACE: Lua: Updating entity. Transform is at x=" .. transform.x)

    -- Implement behavior
    if transform.x < 100 then
        velocity.dx = 5
    else
        velocity.dx = -5
    end
end

-- Register the system with its component dependencies
ecs.register_system(enemy_ai_system, { "Transform", "Velocity" })
--]]
