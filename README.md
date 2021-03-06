# Tracker

[![Build status](https://ci.appveyor.com/api/projects/status/i6sjmosx274gb059/branch/master?svg=true)](https://ci.appveyor.com/project/ManuelMeraz/tracker/branch/master)

[![CodeFactor](https://www.codefactor.io/repository/github/manuelmeraz/tracker/badge/master)](https://www.codefactor.io/repository/github/manuelmeraz/tracker/overview/master)

Track my macronutrients, weight, and compute appropriate TDEE


## Build Instructions (Ubuntu 18.04+ Required)

1. Clone this repository and change into it

`git clone https://github.com/ManuelMeraz/Tracker.git tracker && cd tracker`

2. Install dependencies not installed by conan 

`sudo apt install -y python3-pip python3-setuptools python3-venv doxygen doxygen-doc doxygen-latex doxygen-gui graphviz libgl1-mesa-dev clang-6.0`

3. Set environment variables for this project

`source ./tools/set_env`

4. Install pip dependencies

`pip3 install --upgrade -r requirements.txt`

5. Install configuration settings to build this project correctly with conan

`conan config install https://github.com/ManuelMeraz/conan-config.git`

6. Build the project and download prebuilt dependencies with conan

`./tools/build.py`

7. Run tests, and install plugins to run application

`cd ./build* && make test && sudo make install`


