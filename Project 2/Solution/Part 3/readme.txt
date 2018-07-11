ucontext_t must be a supported variable type, as well as functions such as getcontext, swapcontext,  makecontext
and whatnot must be supported in order for this code to properly work. I wrote a schedule handler as it seemed 
necessary when creating a thread to assign it to the handler to properly create and begin executing the thread. 
I also created a priority system, a very basic 0-3 priority scale. And as I had to create a scheduling system, 
I therby also had to create queue system of some kind, and as such, included 3 thread "waiting lines" when being 
scheduled. I know that I was not asked to do this, but it seemed necessary to me to be able to properly create
a thread and terminate a thread, you must be able to change its status and for that you need a scheduler hand-
ler. 