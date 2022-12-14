Scale = require("Scripts/Scale.lua")
ShowFPS = require("../Common/ShowFPS.lua")

SimpleShader = Shader:new("Simple.vshader")

RedMaterial = Material:new(SimpleShader)
RedMaterial:SetVec3("Color", Vec3:new(1.0, 0.0, 0.0))
RedMaterial:SetVec3("AmbientLighting", Vec3:new(1.0, 1.0, 1.0))

BlueMaterial = Material:new(SimpleShader)
BlueMaterial:SetVec3("Color", Vec3:new(0.0, 0.0, 1.0))
BlueMaterial:SetVec3("AmbientLighting", Vec3:new(1.0, 1.0, 1.0))

CubeMesh = Mesh:new("Cube.vobj")

Count = 20

Camera = Scene:CreateEntity()
Camera:AddComponent(Component.TRANSFORM, {Position = Vec3:new(0.0, 0.0, 25)})
Camera:AddComponent(Component.CAMERA, {NearPlane = 1.0})
Camera:AddComponent(Component.SCRIPT, {Script = ShowFPS})

for X = -Count / 2, Count / 2 do
    for Y = -Count / 2, Count / 2 do
        for Z = -Count / 2, Count / 2 do
            NewEntity = Scene:CreateEntity()

            NewEntity:AddComponent(Component.TRANSFORM, {Position = Vec3:new(X, Y, Z)})
            NewEntity:AddComponent(Component.SCRIPT, {Script = Scale})
            
            if math.abs(Z) % 2 == 0 then
                NewEntity:AddComponent(Component.MESH_RENDERER, {Mesh = CubeMesh, Material = BlueMaterial})
            else
                NewEntity:AddComponent(Component.MESH_RENDERER, {Mesh = CubeMesh, Material = RedMaterial})
            end
        end
    end
end