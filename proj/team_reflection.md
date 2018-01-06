## Table of Contents
* [1. Introduction](#1-göktytorna)
* [2. Quantification](#2-quantification)
* [3. Process](#3-process)
* [4. Use Of Tools](#4-use-of-tools)
* [5. Communication, Cooperation and Coordination](#5-communication-cooperation-and-coordination)
* [6. Work Breakdown Structure](#6-work-breakdown-structure)
* [7. Quality Assurance](#7-quality-assurance)
* [8. Reflection](#8-reflection)

# 1 Göktytorna
## 1.1 Participant List

| Name | Email | Dates active |
| ------------- | ------------- | ------------- |
| Martin Larsson | Martin.Larsson.1498@student.uu.se | 11/12-17-8/1-18 |
| Nicklas Renger  | email  | 11/12-17-8/1-18 |  
| Elisabeth Ryan  | elisabeth.ryan.0075@student.uu.se | 11/12-17-8/1-18 |
| Max Rylander | max.rylander.2608@student.uu.se  | 11/12-17-8/1-18 |
| Ardalan Samimi | Ardalan.Samimisadeh.1548@student.uu.se  | 11/12-17-8/1-18 | 
| Mika Skjelnes | Mika.Skjelnes.1639@student.uu.se | 11/12-17-8/1-18 |

# 2 Quantification

| Type | Info |
|------|------|
| Start date | 11/12-17 |
| End date | 8/1-18 FYLL I |
| Sprints | n/a |
| Lines of code | 672 |
| Lines of tests | 2638 |
| Lines of Demo | 3735 |
| Lines of script | 180 |
| Hours Worked | FYLL I |
| Git commits | 310 |
| Pull Requests | 43 |
| GitHub Issues | 15 |

# 3 Process
## 3.1 Inception

This project was conducted with the agile methodology Kanban with Trello as platform.
We chose Kanban because it was easy to learn and allowed us to work at a distance
through concrete goals represented as notes on Trello.

The application Trello allowed us to create a board with notes, each note has a specifik task that needed to be completed. We had different sections which we moved the notes between depending on its level of completion.
Our sections were "To do", "ToDo: Kod", "Doing", "Review" and "Done". The sections that were
specially adapted for this project were Doing and Review. Doing was limited to
only contain six notes. Because of that the group was forced to complete those tasks
before continuing and limited us to only work on things that had a direct
impact on the current progress. Review was used to guarantee some level
of correctness in the code by forcing another project member to verify that
the code is acceptable. If not it is commented and hopefully fixed. This is
done through GitHubs pull-request function. When a note has passed the review section
it is moved to Done. That action is represented by the pull request is merged
to the master branch.

## 3.2 Implementation
Our planning stategy was less than ideal. We would make loose plans but have no real way of keeping track of what our plans were or who should be doing what to get there. The idea was that it wasnt insanely difficult so we would just do everything one thing at a time. It worked, but far from split the workload evenly over the time frame we had. 
If we were to start over we would probably work harder before christmas, making us less stressed in the beginning of the new year. Alternatively actually work properly over christmas and new years but that seems harder to make happen. Maybe we would make a plan or two as well.

The communication within the team worked fairly well. We would use Trello again, the concreteness of the tasks made it easy to attack the problem.

The strenght of using kanban was that we could always keep track of what was left to be done. The weakness was that it didnt take presidence over us actually talking and deciding things together, this meant it wasnt always updated and therefore not entirely trustworthy. 

Descisions were made in person or over slack, we had meetings consistently which made for perfect arenas to bring up choices that needed to be made by the entire group and problems that needed solving. Key descisions such as how to save all our allocations and what to do about the overhead were the type of descisions where everyone was involved, even if some members were working on something unrelated at the time. Other smaller implementation details were decided with a simple message on slack which was approved by the rest of the group.


# 4 Use of Tools
## Trello
For keeping track of what tasks were left and who did what.
## Slack
For more or less all communication, within the group and with Elias.
## Github
For merging our code, reviewing added code and documenting changes.

# 5 Communication, Cooperation and Coordination
Ardalan was the contact person with Elias, but after our first meeting Elias was added directly to one of our slack channels so anyone could ask him a question if they wanted to. To communicate within the group we used slack, either if someone had a question, felt like they needed a task, added a pull request on GitHub that needed to be reviewed or had any other relevant info or questions. Sometimes it was difficult to get ahold of members, but nobody completely disappeared for too long. We had regular meetings, not always because anyone had anything to say specifically but more to catch up and make plans for the rest of the project.
# 6 Work Breakdown Structure
Since we added all our tasks to Trello it was easy to see what was left to be done. If something had to be completed before the project could be moved along Slack was used to make sure someone was at least working on that part.
Slack was also used for communicating our problems and splitting the work load.

The tasks were of very varying size, but because different memebers of the group have different amounts of experience of C as a language and of programming in general the size of the actual task had little to do with how much work was put into it. Because of this it was also very hard to estimate how long a task would take to complete. It, as usual, completely depends on how many problems you run into on the way. Our utopian goal was to be finished before christmas. Having such a big margin made it less important to estimate the time correctly and more important that the task just got done in time that it didnt stop the rest of the project moving forward.

The work was not completely balanced. Partly because all team members didnt put the same effort in and partly because we are not equally good at programming in C.

# 7 Quality Assurance

| Type | Info |
|------|------|
| Amount of PRs | 43 |
| How many people made PRs | 6 |
| Reviewers | 6 |
| Reviews | 14 |
| Comments | 55 |

We have reviewed all but we have not always used GitHubs function review, in the table we have written the number of GitHub-reviews.

PR:s helped us change formatting so it was all consistent, making sure the code worked on both Linux and Mac, memory leaks were caught and tests were added to catch more cases.

We have switched ruthlessly between lagnuages in our PR:s. Some names were probably felt very relevant to us at the time but seem unclear, such as "changed refmem". Others are easy to understand, like "Första implementation av trädstruktur som pekarregister" the language has inexplicably changed but it is very descriptive. "Unit tests for existing functions" is well-phrased and in english, we did good there.

We learnt that we should probably come up with some sort of standard before we start, naming convention but maybe also have a system for who reviews what or somthing similar. This makes it easier to avoid the situation where one person gets stuck putting time into reviewing when they have other things to work on. Also naming of branches should have been done better. Otherwise git PRs are a very good way to not only make sure the code everyone writes separatly actually comes together but also being able to give eachother feedback before you make changes and keeping track of who does what and what has been done.

# 8 Reflection

Groups biggest win:
Done on time
No members were useless
Communication worked fine

What we did the worst:
Planned badly

## Ratings:
Our method: kanban:
5/7 - Very clear method, made it easy to work.
      A downside was that is was hard to judge how much time was needed for each note.

Our finished product:
6/7 - Our module works and was done on time, very small to no deviations, looks pretty good, we used memory well.

Quality Assurance:
6/7 - We have used our code standard, we made it maintainable and readable.

