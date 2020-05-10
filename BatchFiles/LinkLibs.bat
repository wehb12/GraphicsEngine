REM Link third party libraries lib files and includes

cd ..

mklink /d "\Source\Graphics\glfw-3.3.2\build\src\Debug" "\Source\ThirdParty\Libs\glfw"

cd BatchFiles
