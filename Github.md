# Lazy 🐶 for Git – and you can never guess what happens next!

*Version: 1.0.0*

The following is a short guideline for how we use Github.

### Five (+ five) tips that will CHANGE your life

#### **1)** Pulla, pulla, pulla

Before every coding session – that is, before you sit down and make beautiful <s>love</s> code – make a habit of always pulling the latest changes from the repository.

This prevents you from working on outdated code. Obviously.

###### How do I do it?
Fire up your favourite terminal app and cd to the repo folder and just type in the following command:

```git
$ git pull
```

#### **2)** Branch out with your rooster out

Never (well, almost never) work directly on the ``master`` branch. It is better to create a separate branch where you can rocka loss like a båss.

By working on our own branches we can develop in parallel while keeping working code intact on the ``master`` (or ``develop``) branch, and not interfering with each other.

###### How do I do it?
In the terminal, just type up the following command:

```git
$ git checkout -b <branch_name>
```

This will create a new branch named ``<branch_name>`` and also change to that branch directly.

###### Bonus tip

The ``checkout`` command is a magical one. You can also switch to another (existing) branch using ``checkout``, like so:

```git
$ git checkout <branch_name>
```

#### **3.1)** Commit to committing

Do ``commit`` often, and I really mean **often**. It is generally better to have more, but smaller commits, rather than committing a large block of code (😩!!) at the end of the day.

A good rule of thumb is to make a new commit as soon as you can explain the change in a sentence.

Not only does this make it easier to roll back individual changes if needed, it also allows for you to share code with your colleagues.

#### **3.2)** Related changes are related

Each ``commit`` should preferably include related changes. For instance, if you've squashed a nasty bug, this should be an individual commit.

This can be done, for instance, by separating staging and committing, or committing the file before continuing to work on it.

###### How do I do it?
Stage a file with the ``add`` command, before using ``commit``.
```bash
$ git add modifiedFileOne
$ git add modifiedFileTwo
$ git commit -m "Some message"
```

You can also stage and commit all modified files:

```bash
$ git commit -am "Some message"
```

#### **3.3)** (Committing) half done is not well begun

Try not to commit work that is half-done (if not absolutely necessary).

###### How do I do it?

You just don't!

#### **3.4)** Test your code before commit

This is self-explanatory, but try to always commit code that is tested and works (again, if not absolutely needed to be committed).

#### **3.5)** Push it good

Never forget to ``push`` before the end of the day, to upload all your commits to the remote repository.

###### How do I do it?
After making one (or more commits) just type the following:

```bash
$ git push
```

#### **4)** Don't forget -- Jesus pull requests

When you're satisfied with your changes and want to incorporate those to the ``master`` branch, you should create a ``pull request`` on Github.

But always have someone else to review your changes before merging them.

###### How do I do it?
After pushing all your changes to the repository, do the following:

![PR 1](PR-1.png?)
![PR 2](PR-2.png?)
![PR 3](PR-3.png?)
![PR 4](PR-4.png?)

#### **5)** Issue an issue when a bug is bugging you

Use Github's ``issue`` feature to report bugs, rather than posting it (only) on *Slack* or *Trello*.
