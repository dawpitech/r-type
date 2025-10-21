print("TRACE: Lua: Loading enemy behavior script.")

local function enemy_ai_system(entity)
    print("Processing an entity:")
    -- Access components passed from C++
    local transform = entity.transform
    local velocity = entity.velocity

    print("\tpos x=" .. transform.pos.x)
    print("\tpos y=" .. transform.pos.y)
    print("\tvelocity x=" .. velocity.x)
    print("\tvelocity y=" .. velocity.y)
end

-- Register the system with its component dependencies
ecs.register_system(enemy_ai_system, { "Transform", "Velocity" })
