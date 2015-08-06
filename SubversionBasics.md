
# Introduction #
Welcome to Subversion--our software version control system. It's basically responsible for keeping track of changes to our code in a single repository, so that everyone works off the same files.

Let's go over how to get started.

For further reading, you can always check out the [TortoiseSVN Daily Use Guide](http://tortoisesvn.net/docs/nightly/TortoiseSVN_en/tsvn-dug.html). For the command line tools, check out [\_Version Control with Subversion\_](http://svnbook.red-bean.com), a free online book on the stuff.

## Some Warnings ##
  * The TortoiseSVN client is for Windows only. If you're on Mac or Linux, you'll need to use the [command line instructions](SubversionBasics#Command_Line_(a.k.a._the_hard_way).md).
  * The work network blocks Subversion checkouts. You will not be able to check out code to you work laptop while you're on the work network.
  * All commands need to happen within your checkout directory--svn uses files hidden in your checkout directory to work.

## Subversion Vocabulary ##
Let's go over some of the things you'll do the most often while working with Subversion.

| Trunk | The `trunk/` folder is the code baseline. Code for demos and the final product will come from here. |
|:------|:----------------------------------------------------------------------------------------------------|
| Checkout | A _checkout_ is simply a copy of the Subversion repository on your local hard drive. You'll make your changes to the code within this copy. |
| Updating | While you're working on your local copy, it's possible that other people have committed changes to the trunk. To pull those changes in, you'll need to update your checkout directory. |
| Committing | A _commit_ is the act of pushing the changes within your local copy back to the repository. Typically, whenever you finish a feature, you'll commit it. Typically, your commits will go to the trunk. |
| Branching | When you want to spend more time on a feature, but you don't want to try and keep up with the rest of the repository, you'll branch your code off. Instead of committing against the trunk, you'll commit against your branch. This keeps your code changes from breaking other things until you're ready. |
| Merging | Eventually, all branches need to come back into our trunk. With this operation, you'll combine your branch back into the trunk code once it's stable enough. |

I'll leave branching and merging to the reader--you'll know when you'll need them.

# TortoiseSVN (a.k.a. the easy way) #
TortoiseSVN is a plugin of sorts that integrates Subversion into your conputer's file browser. It's one of the easiest ways to use Subversion.
## Installing TortoiseSVN on Windows ##
I won't go into detail in here how to get and install TortoiseSVN--your IPT leads should be able to help you with that. Only note I'll leave here is that if you'd like to use Subversion from the command line, install the command line packages.

## Checking out a Copy of the WENDE Baseline ##

  1. Open up your **Documents/My Documents** Folder (depends on your version of Windows)
  1. Right-click in the window without selecting any folder or file, then click on **SVN Checkout**. Enter the following in the **Checkout** window.
    * **URL of Repository** is **https://wende-project.googlecode.com/svn/trunk/**
    * **Checkout directory** should end in **wende-project**
    * **Checkout Depth** should be **Fully recursive**
    * **Revision** should be **HEAD revision**
    * Click **OK**.
    * A checkout dialog will show up. When the dialog shows **Completed At revision**, the checkout is complete.
  1. Once the checkout completes, your **wende-project** folder will contain all the files from the Google Code repository.
## Updating Your Checkout Directory ##
  1. Open up your checkout directory.
  1. Right-click in the window without selecting any folder or file, then click on **SVN Checkout**.
  1. An update dialog will show up. When the dialog shows **Completed At revision**, the update is complete.
## Committing Your Changes Back ##
**_NOTE: ALWAYS do an update before you commit changes back--it keeps the trunk clean._**
  1. From the Google Code page, click on the **Source** tab.
  1. Click on the **googlecode.com password** link.
  1. Take note of the password--you'll need it when you check code back in.
  1. Open up your checkout directory.
  1. Right-click in the window without selecting any folder or file, then click on **SVN Commit**. In the **Commit** window, enter a brief message describing what you did (e.g. "Added a 'Search and Destroy' mode to the Laser"). **THIS IS IMPORTANT**. It'll make it much easier to track progress as we go along.
  1. Click **OK**. Once you kick off the commit, you should be asked for a username and password.
    * Your **username** is the email you use for Google Code.
    * Your **password** is the one that you took note of earlier.
  1. A commit dialog will show up. When the dialog shows **Completed At revision**, the commit is complete.

# Command Line (a.k.a. the hard way) #
I'm only half-joking. The command line may seem scary if you're not used to it, but it's pretty straightforward.
## Check if SVN Executable Exists ##
I'll leave it up to you to get the Subversion client installed. But before you go off trying to find it, make sure it isn't already on your system. Issue the following in a terminal:
```
svn
```
If Subversion is installed, you should get something back along the lines of:
```
Type 'svn help' for usage. 
```
## Checking out a Copy of the WENDE Baseline ##
> Open a terminal, and enter the following command
```
svn checkout https://wende-project.googlecode.com/svn/trunk wende-project --username user@example.com
```
Where `user@example.com` is your email address for Google Code. This will create a directory called `wende-project` in your current directory, and will put the wende-project code in it.
## Updating Your Checkout Directory ##
While in your checkout directory, enter the following command:
```
svn update
```
You'll see Subversion download any changes you're missing.
## Committing Your Changes Back ##
**_NOTE: ALWAYS do an update before you commit changes back--it keeps the trunk clean._**
    1. From the Google Code page, click on the **Source** tab.
    1. Click on the **googlecode.com password** link.
    1. Take note of the password--you'll need it when you check code back in.
    1. While in your checkout directory, enter the following command:
```
svn commit -m "Detailed message goes here" --username user@example.com
```
Where `user@example.com` is your email address for Google Code. The text in quotes after the `-m` is your log message, and it needs to describe what you did. **THIS IS IMPORTANT**. It'll make it much easier to track progress as we go along.
You will get prompted for a password, enter the password you took note of earlier.

# Using Qt Creator (a.k.a. the C3 way) #
The C3 team will be using the Qt Creator IDE, which has support for Subversion built-in. Here's how to use it.
## Setting Up the Subversion Plugin ##
  1. If you're on Windows, install TortoiseSVN **with the command-line utilities**.
  1. From the Google Code page, click on the **Source** tab.
  1. Click on the **googlecode.com password** link.
  1. Take note of the password--you'll need it when you check code back in.
  1. From the Qt Creator IDE, go to **File-->Preferences**. Click on **Version Control** in the left-hand pane.
  1. Click on the **Subversion** tab.
  1. For the **Subversion command**, enter <tt>C:\Program Files\TortoiseSVN\bin\svn.exe</tt> (Windows only)
  1. Check the **Authentication** checkbox.
    * Your **Username** is the email you use for Google Code.
    * Your **Password** is the one that you took note of earlier.
  1. Click **OK**.
## Checking out a Copy of the WENDE Baseline ##
Just kidding! You'll have to check out the code using one of the two methods above. :-( This is because the WENDE project contains a mix of code.
## Updating Your Checkout Directory ##
Go to **Tools-->Subversion-->Update Repository**. Yep, that's it!
## Committing Your Changes Back ##
**_NOTE: ALWAYS do an update before you commit changes back--it keeps the trunk clean._**
  1. Go to **Tools-->Subversion-->Commit Project**
  1. A commit dialog will show up. In the **Description**, enter a brief message describing what you did (e.g. "Added a 'Search and Destroy' mode to the Laser"). **THIS IS IMPORTANT**. It'll make it much easier to track progress as we go along.

# Help! #
If you know Subversion well enough to help someone else out, leave your name here. If you need help with Subversion, ask one of these people. :-)
  * Will Trautman
  * Vishal Kotcherlakota