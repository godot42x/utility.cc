target("utility")
do
    set_kind("shared")
    set_languages("c++20")
    add_files("./src/**.cpp")
    add_headerfiles("./src/**.h", { public = true })
    add_includedirs("./src/include", { public = true })
end
