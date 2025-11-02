local function mob_oscillation_system(entity)
    print("test")

    local transform = entity.transform
    local velocity = entity.velocity
    local mobStartPosition = entity.mobStartPosition

    if transform.pos.y >= (mobStartPosition.startY + mobStartPosition.amplitude) then
        mobStartPosition.movingUp = false
    elseif transform.pos.y <= (mobStartPosition.startY - mobStartPosition.amplitude) then
        mobStartPosition.movingUp = true
    end

    if mobStartPosition.movingUp then
        velocity.y = 1
    else
        velocity.y = -1
    end
end

ecs.register_system(mob_oscillation_system, { "Transform", "Velocity", "MobStartPosition" })
