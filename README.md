# Stardust Login Plugin

## Table of Contents

- [Pre-requisites](#pre-requisites)
- [Installation](#installation)
    - [Adding the plugin as a Submodule](#adding-the-plugin-as-a-submodule)
    - [Cloning the repository](#cloning-the-repository)
    - [Manual Installation](#manual-installation)
- [Project Setup](#project-setup)
- [Api Gateway Setup](#api-gateway-setup)

# Pre-requisites

Enable the Web Browser plugin from Unreal engine

# Installation
    
## Adding the plugin as a Submodule

Go to the root of the project and inside the Plugins folder open a terminal
Note: If you don't have a Plugins floder you can created

    git submodule add https://github.com/dvanegas96/stardust-login.git

Then right click the unreal project file and click Generate visual studio project file

![6](https://user-images.githubusercontent.com/92105458/155209654-8511ce22-2a4e-4743-acc8-fbff54d9a8e3.png)

And Open the project

![10](https://user-images.githubusercontent.com/92105458/155210039-bd8d2b81-a889-438f-99a1-237813f91293.png)

## Cloning the repository with example

With version 2.13 of Git and later, --recurse-submodules can be used instead of --recursive

    git clone --recurse-submodules https://github.com/Stardust-Platform/unreal-login.git
    cd unreal-login

With version 1.9 of Git up until version 2.12 (-j flag only available in version 2.8+):
    
    git clone --recursive https://github.com/Stardust-Platform/unreal-login.git   
    cd unreal-login 

If the repo is already clone

    git clone https://github.com/Stardust-Platform/unreal-login.git
    cd unreal-login 
    git submodule update --init --recursive
## Manual Plugin Installation    
    1. Open the project and enable the Web Browser Plugin from Unreal Engine.

![1](https://user-images.githubusercontent.com/92105458/155209268-2e939a2c-2f6f-4e7c-9c3b-99204f1725b6.png)


    2. Close the project and Delete the following files and folders projectName.sln, Intermediate, Saved, Binaries.

![2](https://user-images.githubusercontent.com/92105458/155209351-d23ab5c9-1a52-4679-9148-80e928663a4a.png)


    3. Create a Plugin folder if you don't have one, in the root of the project.

![3](https://user-images.githubusercontent.com/92105458/155209443-7e15c34b-eeae-492d-9629-d7e816aa4799.png)


    4. Copy the StardustLogin folder and paste it inside the Plugins folder.

![4](https://user-images.githubusercontent.com/92105458/155209512-d149b210-a08c-49df-92cf-70e46bd3724f.png)

    5. Right click the Unreal Engine Project File
    6. Click Generate Visual Studio Project Files

![5](https://user-images.githubusercontent.com/92105458/155209592-c2f904e6-aa84-44a8-9d62-5edff3b197d2.png)

    7. Open the project 

![6](https://user-images.githubusercontent.com/92105458/155209654-8511ce22-2a4e-4743-acc8-fbff54d9a8e3.png)
![7](https://user-images.githubusercontent.com/92105458/155209688-612343d0-e4cc-49ad-81b3-615b1b1a83f8.png)
![8](https://user-images.githubusercontent.com/92105458/155209729-66ef01fa-a9fc-4e4c-b52b-098732cd3333.png)

    8. In button right corner of the Content Browser, click View Options, and enable Show Plugin Content

![9](https://user-images.githubusercontent.com/92105458/155209790-9b6f503f-625d-4e39-9f4b-ed0a00b39842.png)

    9. Setup the Project Gamemode with StardustGameMode and the Game Instance class with GI_Stardust

![10](https://user-images.githubusercontent.com/92105458/155210039-bd8d2b81-a889-438f-99a1-237813f91293.png)


    10. Go to the StardustLogin Content folder and drag and drop the StardustWidget folder in the project content.

![11](https://user-images.githubusercontent.com/92105458/155210114-97701c27-a2c1-42ca-8bff-67528f557171.png)


    11. Compile and restart the project.

![12](https://user-images.githubusercontent.com/92105458/155210245-dac00b1a-a07d-4a6d-9376-b98af954d550.png)

# Project Setup

The  Stardust Login  properties can be found in the StardustGameInstance.h file

![Properties](https://user-images.githubusercontent.com/92105458/155211571-d7859660-2910-43e7-8a3d-5265ab89871d.png)


LogoUrl: Set an url with the logo of your brand to show.

![Logo](https://user-images.githubusercontent.com/92105458/155211713-e84529bc-d129-48f6-91f2-480b07f976f5.png)

TermsOfServiceUrl: Url with the terms of services of the company.

PrivacyPolicy: Url with the terms of services of the company.

ApiUrl: Get the Api url by creating a new Rest API in the Api Gateway with a lambda.

Domain: Find the cognito domain under App integration and Domain name

![Domain](https://user-images.githubusercontent.com/92105458/155212686-cd5e7888-378e-46e8-874e-1afce364bf4e.png)


Region: Set the region of were your Amazon cognito pool is located.

![Region](https://user-images.githubusercontent.com/92105458/155211892-c6b11668-255c-4eb2-abd2-df516deb1129.png)

ClientId: Client ID it’s shown General settings then App clients inside the cognito pool

![ClientId](https://user-images.githubusercontent.com/92105458/155212193-75301ec0-1c8a-498c-bab7-39ea687281b2.png)

CallbackUrl: Find your redirect url inside your cognito pool under App integration and then App client settings, as a Callback Url(s)

![CallbackUrls](https://user-images.githubusercontent.com/92105458/155212431-e3c6d478-2f3c-4ac7-a93b-7c00346c859f.png)

# Api Gateway Setup