# GECMooseWorkshop
Repository for the source code and files of the GEC 2018 MOOSE Workshop

## For Contributors from NCSU, UIUC, and INL

### Issues

If discussion is needed for a particular aspect of the workshop, please create an issue for it. This will create a forum for discussion. If changes to the workshop arise from these discussions and can be closed with the new addition, they can be tied to Pull Requests by adding "closes #N" to your Pull Request comment or your Commit Messages, where N is the assigned number for your Issue. When the Pull Request is merged, the corresponding Issue will be closed. If code changes need to reference a particular Issue (but not close it), "refs #N" can be used in your Commit Messages and Pull Requests to link the two together for reference.

### Contributing

In order to make a contribution, please use the following steps:
1) Fork the repository to your own account (according to GitHub, this will keep it private even if you aren't a paid GitHub subscriber).
2) Clone the repository to your local machine using git:
    ```
    git clone https://github.com/[YOUR_USER_NAME]/GECMooseWorkshop.git
    ```
3) Add original repository as a remote repository with the label "upstream":
    ```
    git add remote upstream https://github.com/cticenhour/GECMooseWorkshop.git
    ```
2) Create a new branch in your local repository, and name it something reflecting your change or new addition (the "feature"):
    ```
    git checkout -b my_new_feature
    ```
3) Commit your changes, and then push those commits to your forked GitHub repository.
   ```
   git push -u origin my_new_feature
   ```
   **NOTE1:** "origin" is the default label for the repository you cloned!
   **NOTE2:** You may need to "rebase" your work on top of the current master branch before pushing, if other changes have been made since you originally cloned the project! Your changes **MUST** branch from the most up-to-date commit in master when you make a Pull Request, otherwise they will not be merged! To do this, enter the following commands into your terminal:
   ```
   git checkout master            # ensures that you are on the master branch
   git fetch upstream             # downloads any changes in the original repository, and updates upstream/master
   git rebase upstream/master     # rebases your master branch onto the history of upstream/master
   git checkout my_new_feature    # checks out to your feature branch
   git rebase master              # rebases your feature branch onto your updated master branch
   git push origin                # send your updates to your repo on GitHub
   ```
4) On GitHub create a Pull Request in the original repository using its master branch as your base and your repository's feature branch as the head. This will create a forum where collaborators can discuss and make comments on the changes before they are merged, giving the originating collaborator opportunities to make further changes. When a consensus on the changes has been reached, the pull request can be merged.

### Need More Help?

See https://help.github.com/ if further help is needed on using Git and GitHub. A Git Cheat Sheet is provided at https://services.github.com/on-demand/downloads/github-git-cheat-sheet/

## How To Build
1) Download and install a LaTeX distribution (TeX Live or MikTeX are good choices for Windows, MacTeX for MacOS, and many more exist...).
2) If you want to build the presentation using a visual environment, ensure that your distribution came with one or download one (**NOTE** TexStudio seems to be the best IDE to build the presentation with, as others seem to produce errors pertaining to interpretation of tabs/spaces - a quirk of the internal text editors in these programs).
3) Build the document:

   * If you are using a visual environment, refer to the build instructions that come with that program. See the NOTE below - it also applies to builds using editors.

   * If you are using the terminal (in Linux or MacOS) or command prompt (Windows), use the following command in the directory where the workshop .tex file is located:

      ```
      pdflatex -synctex=1 -interaction=nonstopmode "keniley_crane_aps-gec".tex
      ```

      **NOTE:** Because the LaTeX compiler requires cross-referencing information to build the table of contents, you must run this command twice to ensure that the table of contents is built and visible for presentation navigation!
