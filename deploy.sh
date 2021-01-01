rm .git

git init
git add -A
git add -f .gitignore
git commit -m 'Deploy'

git push -f git@github.com:lnq99/AA.git master:master
git push -f gitlab@git.iu7.bmstu.ru:ln18iu10/aa.git master:master
