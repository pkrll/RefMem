# 1 Göktytorna
## 1.1 Participant List

| Name | Email | Dates active |
| ------------- | ------------- | ------------- |
| Martin Larsson | Martin.Larsson.1498@student.uu.se | 11/12-17-8/1-18 |
| Nicklas Renger  | email  | 11/12-17-8/1-18 |  
| Elisabeth Ryan  | ebethryan@gmail.com | 11/12-17-8/1-18 | 
| Max Rylander | email  | 11/12-17-8/1-18 | 
| Ardalan Samimi | email  | 11/12-17-8/1-18 | 
| Mika Skjelnes | email | 11/12-17-8/1-18 |

# 2 Quantification

| Type | Info |
|------|------|
| Start date | 11/12-17 |
| End date | 8/1-18 FYLL I |
| Sprints | 0 or 1 FYLL I |
| Lines of code | FYLL I |
| Lines of tests | FYLL I |
| Lines of script | FYLL I |
| Hours Worked | FYLL I |
| Git commits | FYLL I |
| Pull Requests | FYLL I |
| GitHub Issues | FYLL I |

# 3 Process
## 3.1 Inception
This project was conducted with the agile methodology Kanban with Trello as platform.
We chose Kanban because it was easy to learn and allowed us to work on a distance
through concrete goals represented as notes on Trello.

The application Trello allowed us to create a board with post it notes. There
we had different sections depending on which stage current note were on. The
sections were To do, ToDo: Kod, Doing, Review and Done. Those sections which were
specially adapted for this project was Doing and Review. Doing is limited to
only contain six notes. Thereby the group is forced to complete those tasks
before continuing and limit us to only work with stuff that has a direct
effect on the current progress. Review is used to guarantee some level
of correctness in the code. By forcing another project member to verify that
the code is acceptable. If not it is commented and hopefully fixed. All this is
done through GitHubs pull-request function. When a note has passed the review section
it is moved to Done. That action is represented by the pull request is merged
to the master branch.
# 4 Use of Tools
## Trello
For keeping track of what tasks were left and who did what
## Slack
For more or less all communication, within the group and with Elias.
## Github
For merging our code, review added code and documenting changes.
# 5 Communication, Cooperation and Coordination
Ardalan was the contact person with Elias, but after our first meeting Elias was added directly to one of our slack channels so anyone could ask him a question if thye wanted to. To communicate within the group we used slack, either if someone had a question, felt like they needed a task, added a pull request on git hub that needed to be reviewed or had any other relevant info or questions. Sometimes it was difficult to get ahold of members, but nobody completely disappeared for too long. We had regular meetings, not always because anyone had anything to say specifically but more to catch up and make plans for the rest of the project.
# 6 Work Breakdown Structure
Since we added all our tasks to trello it was easy to see what was left to be done. If something had to be completed before the project could be moved along slack was used to make sure someone was at least working on that part.
Slack was also used for communicating problems and splitting the work load, as trello makes it hard to discuss, it is more of a documenting tool.
The tasks were of very varying size, but because different memebers of the group have different amounts of experience of c as a language and of programming in general the size of the actual task had little to do with how much work was put into it. Because of this it was also very hard to estimate how long a task would take to complete. It, as usual, completely depends on how many problems you run into on the way. Our utopian goal was to be finished before christmas. Having such a big margin made it less important to estimate the time correctly and more important that the task just gets done in time that it didnt stop the rest of the project moving forward.
The work was not completely balanced. Partly because team members were at times busy with other things during christmas and partly because we are not equally good at programming in c.

# 7 Quality Assurance

## Where is the spec unclear?
We had to decide for ourselves how to keep the overhead down, we solved this by creating a list with the pointers to our destructors and only sending the index to the destructors place in the list with our object and not the entire adress.
How to save all of our allocations was also something we had to reason about so we could find the smartest way to keep all our info in a way that doesnt use unreasonble resource. We started off with a tree but later changed our minds and implemented a queue. We realised that the time complexity didnt matter when we changed our integer type to a smaller one, and a queue is much easier to work with.
## How many PRs did we do, by how many people, how many reviews, how many comments on prs?
## what did we change beause of PRs
## What were our best and worst pr names?
We have switched ruthlessly between lagnuages in our PR:s. Some names were probably felt very relevant to us at the time but seem unclear, such as "changed refmem". Others are easy to understand, like "Första implementation av trädstruktur som pekarregister"
the language has inexplicably changed but it is very descriptive. "Unit tests for existing functions" is well-phrased and in english, we did good there.
## What did we learn?
We learnt that we should probably come up with some sort of standard before we start, naming convention but maybe also have a system for who reviews what or somthing similar. This makes it easier to avoid the situation where one person gets stuck putting time into reviewing. Otherwise git PRs are a very good way to not only make sure the code everyone writes separatly actually comes together but also being able to give eachother feedback before you make changes and keeping track of who does what and what has been done.

# 8 Reflection
