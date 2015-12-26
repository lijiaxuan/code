#cp -r /home/obsidian/program/ljx /home/obsidian/program/git
tar1name=$(date +%Y_%m_%d_ljx.zip)
tar2name=$(date +%Y_%m_%d_xp.zip)
cd ~/program/git
#rm *.tgz
find ../ljx -type f -not \( -path '*/matpower/*' -or -path '*/STM32F10x_FWLib/*' \) \( -name "*.c" -or -name "*.h" -or -name "*.py" -or -name "*.nb" -or -name "*.m" -or -name "*.md" \) |xargs zip $tar1name -P 928107lijiaxuan
cd /home/obsidian/virtualbox/xp/code
find ./ -type f -not \( -path '*/STM32F10x_FWLib/*' -or -path '*B_GPS*' -or -path '*GPSControl' \) \( -name "*.c" -or -name "*.h" -or -name "*.py" -or -name "*.nb" -or -name "*.m" -or -name "*.md" \) |xargs zip ~/program/git/$tar2name -P 928107lijiaxuan
cd ~/program/git
git add .
git commit -a -m "store"
git push -u origin master
