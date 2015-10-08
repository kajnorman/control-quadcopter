# Day to day journal #



Day to day journal describing the jobs done, trying to implement the following skelleton.

  * **Attendees**
  * **Secretary**
  * **time and date**
  * **duration**
  * **purpose**
  * **background**
  * **enable a restoring of the situation. (redo)**
  * **commands (and how they work)**
  * **Tests (proof)**
  * **Problems encountered**
  * **Conclussions**
  * **Plan of next steps**



# Section 1 #

  * ## Attendee and Secretary ##
> > == Kaj Norman Nielsen
  * ## time and date ##
> > 25-05-2013 21:30- 22:56
  * ## duration ##
  * ## background ##
    * As part of optimizing the dynamic behaviour of the controllop a certain delay in response has been encountered in the "Electronic Speed Controller"(ESC). This is mentioned in [the arducopter forum](http://diydrones.com/forum/topics/arducopter-esc-pwm-update-rate). They are discussing how to controll the ESC using a faster pwm frequency than the normal (50Hz)
  * ## purpose ##
    * The purpose is to examine the immediate solutions to this
      * First preparing what may be done (no hardware available at home)
      * Make a plan of how this is tested
      * The purpose is also to test this wiki-system for journaling
  * ## Controlling the ESC at this time ##
    * Is done using the  [ServoBlaster](https://github.com/richardghirst/PiBits/tree/master/ServoBlaster) system. This is at this time running as a daemon.(It could be running as a kernel module). The PWM output for the ESC is at different occasions meassured to be running at a frequency of about 50 Hz.
    * ## Looking into using  the Servoblaster at frequencies beyond 50 Hz ##
      * When  browsing the README file in [ServoBlaster](https://github.com/richardghirst/PiBits/tree/master/ServoBlaster) I see two parameters that are to be controlled : cycle\_ticks and tick\_scale.
  * ## Problems encountered ##
    * I can not find any examples on how to set up the servoblaster for 400 Hz control..  Will have to figure it out myself.
  * ## Conclussions ##
    * Wiki-journalling seems ok. (Pictures may be a problem)
    * If the Servoblaster can't support more than 50 Hz it may be dropped.
    * Strange that noone has used the servoblaster for a quadcopter.. Or maybe not!
  * ## Plan of next steps ##
    * Set up a test to prove
      * The normal frequency of the pwm signal from servoblaster
      * that there is a delay inside the (ESC)
      * meassure this delay
      * try to set different values for cycle\_ticks and tick\_scale and figure out how they work on the output signal

# Section 2 #
  * ## Attendee and Secretary ##
> > == Kaj Norman Nielsen
  * ## time and date ##
> > 02-07-2013 07:51 - 08:05
  * ## duration ##
  * ## background ##
    * As part of optimizing the dynamic behaviour of the controllop a problem using the stdlib function delay is encountered. When using delay(); a voulenteer time share is started meaning that the operating system decides when you will be able to run again. This may be solved using XXXXX. That proves to give a "steady pace sampling frequency".

  * ## purpose ##
    * The first purpose is to implement XXXX in the code and test this
    * The second purpose is to examine the immediate solutions to delays in the controll loop
      * First preparing what may be done (no hardware available at home)
      * Make a plan of how this is tested
  * ## The code that implements a "steady pace sample frequency " at this time ##
    * Is done :
  * ## Plan of next steps ##