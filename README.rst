ua-ios-pod
==========

ua-ios-pod does the following things:

- Provides a staging area for the UrbanAirship-iOS-SDK podspec (which lives in the Cocoapods Specs repo).
- Provides a pod sample app PodSample with accompanying podfile.  

How to use PodSample:
^^^^^^^^^^^^^^^^^^^^^

- Navigate to PodSample's podfile and run 'pod install'
- Open the PodSample.workspace that is created during install once the installation is complete

Editing the Podfile:
^^^^^^^^^^^^^^^^^^^^

The podfile can be edited to include any of the following subspec options:

- Core: Installs the UA static library, headers and required resources
- CommonUI: Installs Common UI components
- PushUI: Installs Core, Common UI and Push UI components
- InboxUI: Installs Core, Common UI and Inbox UI components
- All: Installs Core, Push UI components and Inbox UI components

Sample podfile contents:
^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: json

   source 'https://github.com/CocoaPods/Specs.git'
   # fetch source and podspec from local ua-ios-pod-dev directory
   pod 'UrbanAirship-iOS-SDK/All', :path => ".."



