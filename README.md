# COMP1917-mechanicalTurk-JandL

### next steps would be to find out the best amount of arcs and campuses to build before just focusing on go8's and spinoffs, ive done a new working path that goes into the middle of the board to start with i think if we can try to aim for specific vertices to build arcs at if we can would be good like aim for a certain students square, we are getting alot of THD's cause ive set the exchange for that at 5 so we can get go8's we could set the exchange to be 3 until we get say 4 campuses then increase it so were not losing students everytime a 7 is rolled and we could try and build a go8 on the best vertice that we want so it doesnt just randomly upgrade the worst place campus and doesnt get utilized, if you think of some more ideas just keep writing on this post.

Ok so what i have in mind from this is:
1. the amount of arcs/campuses to build before focusing on GO8s/spinoffs would probably be about when we have about 60-70% 
   of the max arcs/campuses built
2. to work out the best vertices to build stuff, i'll try to come up with some sort of function to figure out what 
   disciplines are around the vertex, then we can use if statements to figure out which vertex we are going to aim for.
   im thinking of using the getDisciplines function but im not sure how i can relate the regionIDs with the new workingpath 
   now, i'll see if i can find some pattern, if not i might have to hard code it. 
3. yeah it might be a good idea to reduce the exchange to 3 or 4 until we get 4 campuses, then maybe buil 1 GO8 then a campus
   in an alternating manner until we reach the number of campus that we set from step 1

List below - in the planned order you will implement them (first at the top) - your ideas for how you will make your mechanical turk behave.  Give brief reasons why you think each feature is important, and why you have chosen to implement them in this order.

1.  Mr Pass - starts spinoff if has resources, otherwise passes.  Why first? This is required to be the first Turk everyone submits.
2.  build an arc towards the center if has resources, otherwise check 1. Second, because it allows more edges to put campuses or block others from creating more arcs later in the game.
3.  build a campus if the are enough arc roads and resources to allow building of a campus, otherwise check 2. 3rd because arcs need to be built first before a third campus can be built, campuses generate students that are used to build.
4.  build GO8 if has resources and 70% of the board is filled up and total no. of GO8's owned<8, 4th, because it is used to improve campuses. hence, should be build afterwards.


Write below the plan you and your Turk Pair partner have devised to ensure you will be an effective team:

We will :
- come up with a more in depth strategy for our mechanical turk in our next meeting
- come up and split up the functions that will be needed in our AI
- implement the functions into a strategy
- reiterate improvement process
- win the competition!
