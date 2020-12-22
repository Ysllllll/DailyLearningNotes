[Udacity课程主页](https://classroom.udacity.com/courses/ud456)

#### 访问远程仓库的方式

- 访问远程仓库的方式有多个:
  - 使用 ```URL```
  - 使用系统文件的路径

虽然你也可以在自己的文件系统上创建远程仓库，但这种做法极少见。到目前为止，访问远程仓库的最常见方式是通过位于 ```Web``` 上的仓库的 ```URL```

#### **```git remote```**

```shell
git remote add origin https://github.com/Ysllllll/DailyLearningNote.git
```

- 首先，这条命令包含一个子命令 ```add```
- 这里使用了 ```origin``` 一词，设置了我们之前所说的简写名
  - 记住， ```origin``` 一词并没什么特殊性。
  - 如果你想将它改为 ```repo-on-GitHub```，那么只需（在运行命令之前）将 ```"origin"``` 改为 ```"repo-on-GitHub"```：
  
  ```shell
  git remote add repo-on-GitHub https://github.com/Ysllllll/DailyLearningNote.git
  ```

- 第三，添加了仓库的完整路径（即 ```Web``` 上的远程仓库 ```URL```）

我可以使用 ```git remote -v``` 来验证我已经正确添加了远程仓库。

小结：远程仓库与你使用的本地仓库一样，只是存储在不同的位置。要管理远程仓库，可使用 ```git remote``` 命令，你可以连接到多个不同的远程仓库。简写名是用于指代远程仓库位置的名称。通常该位置为 ```URL```，但也可能是同一台计算机上的文件路径。

- ```git remote add``` 用于添加到新的远程仓库的连接。
- ```git remote -v``` 用于查看远程仓库与连接之间的详细信息。

#### **```git push```**

小结：```git push``` 命令用于从本地仓库向远程仓库推送 ```commit``` 。

```shell
git push origin master
```

该命令需要：

- 你想向其推送 ```commit``` 的远程仓库的简写名：```origin```
- 包含你想推送的 ```commit``` 的分支：```master```

#### **```git pull```**

小结：如果你想在本地仓库中包含远程仓库中的更改，那么你要_拉取_这些更改。要使用 ```Git``` 这样做，你需要使用 ```git pull``` 命令。你告诉 ```Git``` 你想获取修改的远程仓库的简写名以及包含你需要的更改的分支：

```shell
git pull origin master
```

在运行 ```git pull``` 时，会发生以下活动：

- 远程分支上的 ```commit``` 会被复制到本地仓库
- 本地跟踪分支（```origin/master```）移到指向最新的 ```commit```
- 本地跟踪分支（```origin/master```）合并到本地分支（```master```）

另外，可以在 ```GitHub``` 上手动添加更改（但不建议这样做，所以别这样做）。

#### **```git fetch```**

小结：你可以将 ```git pull``` 命令想象成执行两步操作：

- 获取远程更改（这会将 ```commit``` 添加到远程仓库，并移动跟踪分支指向它们）
- 将本地分支与跟踪分支合并

这里```git fetch``` 命令只执行上面的第一步。它只检索 ```commit``` 和移动跟踪分支。它_不会_将本地分支与跟踪分支合并。提供给 ```git pull``` 的同样信息也要传递给 ```git fetch```：

- 远程仓库的简写名
- 包含要取回的 ```commit``` 的分支

```shell
git fetch origin master
```

#### **```git log 扩展```**

小结：```git log``` 命令非常强大，你可以使用它来深入了解关于仓库的很多信息。而且，它在探索有关你与他人协作的仓库的信息这一方面很实用。你可以使用 ```git log``` 完成以下任务：

- 使用 ```git shortlog``` 按作者对 ```commit``` 分组

    ```shell
    git shortlog
    ```

- 使用 ```--author``` 选项筛选 ```commit```

    ```shell
    git log --author="Richard Kalehoff"
    ```

- 使用 ```--grep``` 选项筛选 ```commit```
  
   ```shell
   git log --grep="border radius issue in Safari"
   ```
