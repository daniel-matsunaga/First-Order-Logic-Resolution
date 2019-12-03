# First-Order-Logic-Resolution

My program takes an input of queries and a knowledge base of facts and tries to infer if the quereies are either true or false based on what it knows. Please see the project description PDF for a more precise description and parameters.

I used a Set of Supports of all the atomic sentences in the knowledge base to do a Depth-First search and add all the new sentences that were resolved from the atomic sentences to the Set of Supports. This allowed my program to work with a subset of the knowledge base, which was consistent and narrowed down my search dramatically. 
