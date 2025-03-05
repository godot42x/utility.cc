target("utility.cc")
do
    set_kind("shared")
    set_languages("c++20")
    add_files("./src/**.cpp")
    add_headerfiles("./src/**.h", { public = true })
    add_includedirs("./src/include", { public = true })


    on_config(function(target)
        if is_host("window") then
            if target:get("kind") == "shared" then
                print(target:name(), "with microshit rule >>")
                local normalizeName = target:name():upper():replace("%.", '_')
                print("normalizedName:", normalizeName)
                target:add("defines", "BUILD_SHARED_" .. normalizeName .. "=1")
            end
        end
    end)
end


target("utility.cc.test")
do
    if bDebug then
        print("add test unit:", target_name)
    end
    set_group("test")
    add_deps("utility.cc")
    set_kind("binary")
    add_files("test/*.cpp")
    target_end()
end
