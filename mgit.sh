#cp -r /home/obsidian/program/ljx /home/obsidian/program/git
dataname=$(date +%Y%m%d)
echo $datename
cd ~/program/git
#rm *.tgz
find ../ljx -type f \( -name "*.c" -or -name "*.h" -or -name "*.py" -or -name "*.nb" -or -name "*.m" -or -name "*.md" \) |xargs tar czf $datetime"ljx.tgz"
cd /home/obsidian/virtualbox/xp/code
find ./ -type f \( -name "*.c" -or -name "*.h" -or -name "*.py" -or -name "*.nb" -or -name "*.m" -or -name "*.md" \) |xargs tar czf /home/obsidian/program/git/$datetime"xp.tgz"
cd ~/program/git
git add .
git commit -a -m "store"
git push -u origin master
