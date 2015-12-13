#cp -r /home/obsidian/program/ljx /home/obsidian/program/git
cd ~/program/git
rm *.tgz
find ../ljx -type f \( -name "*.c" -or -name "*.h" -or -name "*.py" -or -name "*.nb" -or -name "*.m" -or -name "*.md" \) |xargs tar czf ljx.tgz
cd /home/obsidian/virtualbox/xp/code
find ./ -type f \( -name "*.c" -or -name "*.h" -or -name "*.py" -or -name "*.nb" -or -name "*.m" -or -name "*.md" \) |xargs tar czf /home/obsidian/program/git/xp.tgz
cd ~/program/git
git add .
git commit -a -m "store"
git push -u origin master
