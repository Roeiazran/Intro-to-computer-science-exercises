# Relations Tree Manager.

This program manages a family tree (see diagram) and focuses on recursive programming, memory allocation and deallocation, structures, and pointers.
The program supports the following features:
1. Add a family head:
A person is considered a family head if they do not have known parents in the tree. Each person must have a unique name. The program supports names of unlimited length using dynamic memory allocation.
2. Marry two people:
Two people can be married if they do not share a common ancestor, are of the appropriate age (18 or older), and neither is already married.
3. Add a new descendant:
Married couples can register a new child. A new baby is always assigned an age of 0.
4. Print a family tree:
Given a family head, the program prints their family tree starting from them. Descendants are printed in the order they were added (which also corresponds to their age). Each generation is indented with a tab (or space), and each couple is printed on the same line.

Example:

```
King Charles (74) - Princess Diana (61)
    Prince William (41) - Catherine Middleton (42)
        Prince George (10)
        Princess Charlotte (8)
        Prince Louis (5)
    Prince Harry (39) - Meghan Markle (41)
        Prince Archie (4)
        Princess Lilibeth (2) 
```

5. Add a specified number of years to any person in the tree.

6. Count the total number of people in the tree.

7. Print relatives at a given degree of kinship, sorted alphabetically by name.

    * Given a person’s name and a degree of closeness, all relatives at that degree will be printed in the same format.

Degree 0: The person themself.
Degree 1: The person and their siblings.
Degree 2: The person and their cousins who share the same grandparents.
Degree 3: The person and all relatives who share a great-grandparent.

And so on. 

#

For demonstration, i have created the following diagram:

![Diagram](relationTree.png)

To test the code using a family tree like the one in the diagram, place input.txt in the root directory and run:

```
gcc familyTree.c
```
and then run:
```
./a.out < input.txt
```
You can also project the output into a result.txt file by executing:
```
./a.out < input.txt > res.txt
```

In this test, we output the family trees of Yaniv and Yuval, the second-degree relatives of Eran, and the third-degree relatives of Tamar.

```
Yaniv (67) - Yarden (64)
	Shalom (44)
	Dan (44) - Sharon (74)
		Eitan (22)
		Eran (22) - Michal (74)
			Asaf (0)
		David (22)
	Yeal (44)
	Daniel (44)
	Stav (44) - Alon (44)
		Shani (22)
		Sivan (22) - Yarin (74)
			Yaara (0)
			Tamar (0)

Yuval (69) - Elad (64)
	Alon (44) - Stav (44)
		Shani (22)
		Sivan (22) - Yarin (74)
			Yaara (0)
			Tamar (0)
	Einav (44)

Cousins:
David
Eitan
Eran
Shani
Sivan


Cousins:
Asaf
Tamar
Yaara

```

You can also run your own tests by modifying the input.txt file. For example, to print Dan’s subtree, simply append the following to the input file:

```
4
Dan
```

In this way, you can append any command you want.

