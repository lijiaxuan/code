#cp -r /home/obsidian/program/ljx /home/obsidian/program/git
tar1name=$(date +%Y_%m_%d_ljx.tgz)
tar2name=$(date +%Y_%m_%d_xp.tgz)
cd ~/program/git
#rm *.tgz
find ../ljx -type f -not -path '*/matpower/*' \( -name "*.c" -or -name "*.h" -or -name "*.py" -or -name "*.nb" -or -name "*.m" -or -name "*.md" \) |xargs tar czf $tar1name
cd /home/obsidian/virtualbox/xp/code
find ./ -type f \( -name "*.c" -or -name "*.h" -or -name "*.py" -or -name "*.nb" -or -name "*.m" -or -name "*.md" \) |xargs tar czf /home/obsidian/program/git/$tar2name
cd ~/program/git
git add .
git commit -a -m "store"
git push -u origin master
