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
In this project, we used the agile methodology Kanban, with Trello as platform. We chose Kanban because it was easy to learn, and also made it easier to visualize and break down the project using Kanban cards.

Before implementing the process, one group member was first tasked to research the method, to get a better understanding of how it works, and later gave a small presentation to the rest of the group.

The only tool needed to implement Kanban was, for us, Trello, which allowed us to create a board with notes, where each note represented a specific task that needed to be completed.

These notes should include for instance a brief description of the task, who's assigned to it and an estimated time for completing it.

## 3.2 Implementation

Our Trello board was divided into different sections, between which we moved the notes depending on their level of completion.

The sections were:

* Todo
* Todo: Kod
* Doing
* Review
* Done

The sections that were especially adapted for this project were *Doing* and *Review*.

*Doing* was limited to only contain six notes. This meant that the group could not take on too many tasks without completing them, and limited us to work only on things that had a direct impact on the current progress.

*Review* was used to guarantee some level of correctness in the code by forcing another project member to verify that the code is acceptable. This was done through GitHub's pull request functionality. When a pull requests was approved and merged, the related note would be moved from *Review* to *Done*.

We also chose not to give a time estimation to each card. Partly because the team members have varying experience in programming – meaning that while person A completes a task in 1 hour, person B might require 6 hours – making it impossible to estimate how long a task should take.

This is in part related to our planning strategy – or, rather, the lack of one.

Our planning was less than ideal. For instance, the Trello cards did not really have any internal priority (other than perhaps in our heads), making it hard to know which tasks are more important to get done. Had we planned better, we could've avoided bottlenecks.

We did not have a team leader, who could hand out assignments. Instead, each member had a larger responsibility to actually get work done.

As we figured the project wasn't insanely difficult, we did everything one task at a time. While it worked out in the end, the workload was far from evenly split over the time frame that we had.

If we were to start over, we would probably work harder before the Christmas break, making us less stressed in the beginning of the new year. Alternatively, work during the break and New Years.

The strength of using Kanban was that we could always keep track of what was left to be done. The weakness of our implementation was the lack of appropriate planning. For instance, each task should've had a deadline.

The communication within the team worked fairly well. We would use Trello again, the concreteness of the tasks made it easy to attack the problem.

Decisions were made in person or over Slack. We had meetings consistently, which made for perfect arenas to bring up choices that needed to be made by the entire group and problems that needed solving.

Key decisions, such as how to save all our allocations and what to do about the overhead, were the type of decision where everyone was involved with making, even if some members were working on something unrelated at the time. Other, smaller implementation details were decided with a simple message on Slack.

# 4 Use of Tools

## Trello
As previously mentioned, we used Trello for keeping track of the progress, to easily see what needed to be done, and who was assigned to do what.

## Slack
Slack was used for communication, both within the group and with Elias, especially during the Christmas break when we couldn't meet in person.

## Github
For merging our code, reviewing added code and documenting changes.

## Astyle
For formatting the code.

# 5 Communication, Cooperation and Coordination
Overall, we feel that we communicated fairly well during the course of the project. We tried to have a clear dialogue and always be straight forward with each other.

Even though it sometimes was difficult to get ahold of team members, nobody completely disappeared for too long.

We used Slack to communicate and coordinate. There, members could post questions, ask for assignments, push each other to finish a task, and so on. We had also invited our coach **Elias Castegren** to Slack and Trello, making it easier for us to get help if needed.

Meetings took place both in person, but also through Slack, especially during the Christmas break.

We tried to have meetings on a regular basis, or at the very least keeping each other posted on the progress made. We had a total of 6 bigger meetings, and kept each other up-to-date with what we were doing several times a week. As always, this could have been done better – for instance, always having 5 minute meetings (over Slack) every or every other day for just status updates.

All important decisions were made during these meetings, where we would discuss topics and together find a solution we all could agree on. We never really had any big disagreements.

The only time the communication (somewhat) broke down was a week during the Christmas break. No real progress was made during this week, as most were waiting for one team member to finish a task. This person, however, were away at that time, and had not communicated it clearly to the others. This could've been avoided had we either had a project leader (who could've re-assigned tasks) or just better foresight.

What we have learned from this experience is that we should (1) always have daily meetings (no exceptions) and (2) not be afraid of taking over an important task that everyone is waiting for.

# 6 Work Breakdown Structure
We started by trying to break down the problem into smaller problems, and from that create cards on Trello.

Depending on the task, it would fall into one of two categories. All tasks related to the actual product were put in the section "ToDo: Kod", while all others (more related to the project itself and the project report) were put in the section "ToDo".

The product related tasks were mainly implementation or improvements of functions and bug fixes while project related tasks could include as diverse tasks as writing the report, meetings or deciding on a coding convention.

Over time, the amount of tasks and cards grew the more we worked on the project.

Since we added all our tasks to Trello it was easy to see what was left to be done. If something had to be completed before the project could be moved along, Slack was used to make sure someone was at least working on that part.

Slack was also used for communicating our problems and splitting the work load.

The tasks were of very varying size, but because different members of the group have different amounts of experience of programming, the size of the actual task had little to do with how much work was put into it. Because of this it was also very hard to estimate how long a task would take to complete. As usual, it completely depends on how many problems you run into on the way.

Our goal was to be finished before Christmas. Having such a big margin made it less important to estimate the time correctly and more important that the task just got done in time so that it didn't stop the rest of the project moving forward.

The work was not completely balanced. Partly because all team members didn't put the same effort in and partly because we are not equally good at programming in C.

# 7 Quality Assurance

Before our first meeting, each member separately had to read the specification so that we would all be on the same page from the start. We didn't have some grand strategy to make sure we correctly understood the specification, other than internal discussions and reading the relevant parts more closely.

We decided early on that each person tasked with implementing a certain function was also responsible for writing the (initial) unit tests, meaning that if person A was to write the function ``allocate``, the same person should also write the unit tests. The tests should not only pass, but also not leak memory (using ``valgrind``).

Of course, this turned out to be easier said than done in the beginning, as some functions were not written yet. This meant that we sometimes had to break our own rule and let there be memory leakage in our tests.

 Because tests are hard to write, we also had to rewrite or add more tests during the course of the project, to make sure that we really tested the functions.

The integration testing came fairly late, after the Christmas break, when we first finished the demonstration, but the same idea applied there. Whenever a module, for instance ``List``, was rewritten to use our garbage collector-system, it had to pass the (existing) test. However, the tests needed to be modified to use ``retain`` and ``release``.

##### Pull Request Strategy

| Type | Info |
|------|------|
| Amount of PRs | 43 |
| How many people made PRs | 6 |
| Reviewers | 6 |
| Reviews | 14 **<sup>A</sup>** |
| Comments | 55 |

Early on, we decided on a specific workflow for Git, which we documented in our guideline posted in the repo.

In short, we tried to always work on our own branches when implementing a function, before merging it with the master branch. Apart from that, we also decided on what decided on what to look at when approving pull requests, resulting in the following:

> ##### What to look at when reviewing?
> Use the following as a guide for when reviewing someone else's code (but also before creating a pull request).
> * Is the code documented correctly?
> * Are there any unit tests?
> * Does the code run correctly on your own computer?
> * Are there any parts in the code that are not executed, or returns unused results?
> * Does it follow our coding convention?
> * Does the code apply lagom defensive programming?

Before a new pull request was to be created, the team members were instructed to make sure the changes passed all of the above points, making it easier for the reviewer. Of course, sometimes we forgot to check some of the points, and other times some of the points were not even applicable.

The reviews helped us mostly catch missing unit tests or documentation, and sometimes not properly formatted code.

Even though we had a guideline for how to work with Git, it turned out not to be comprehensive. We learned later on that we should've decided on some sort of naming convention for both branch names and pull requests.

We have switched ruthlessly between languages in our pull requests. Some names probably felt very relevant to us at the time, but are now unclear and confusing, such as "changed refmem".

Others are easy to understand, like "Första implementation av trädstruktur som pekarregister". The language has inexplicably changed, but it is very descriptive. "Unit tests for existing functions" is well-phrased and in English, we did good there.

Apart from a naming standard, we should've had a better system for who reviews what. According to our own guideline the person creating a pull request should have also requested a review from someone. We rarely followed this rule, for some reason.

**[ A ]** *All PR:s have been properly reviewed, but because we have not always used Github's Review-functionality, the number in the table does not correspond to the actual number of pull requests.*

# 8 Reflection

**The teams biggest win:**  In truth, we consider our biggest win to be that we completed the project in time, and implemented everything. The communication between team members, and the fact that we had no drop outs, should be considered a win, too.

**The teams biggest fail:** We feel that did poorly when it comes to planning properly (we wanted to be done before Christmas).

## Ratings:
**Our process:** (Kanban)

5/7 - Very clear method, made it easy to work. A downside was that is was hard to judge how much time was needed for each note.

**Our delivered product:**

6/7 - Our product works and was done on time, very small to no deviations, looks pretty good, and we used memory well.

**Quality Assurance:**

6/7 - We used pull requests properly, always had testing in mind, tried to write good, maintainable and readable code.
