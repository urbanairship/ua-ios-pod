ua-ios-pod
==========

ua-ios-pod does the following things:

- Provides a staging area for the UrbanAirship-iOS-SDK podspec (which lives in the CocoaPods Specs repo).
- Provides a pod sample app PodSample with accompanying podfile.

How to use PodSample:
^^^^^^^^^^^^^^^^^^^^^

- Navigate to PodSample's podfile and run 'pod install'
- Open the PodSample.workspace created during install once the installation is complete

Sample podfile contents:
^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: json

    source 'https://github.com/CocoaPods/Specs.git'
    use_frameworks!

    # fetch source and podspec from local ua-ios-pod-dev directory
    target "PodSample" do
    pod 'UrbanAirship-iOS-SDK', :path => ".."
    end


Bitcode Support:
^^^^^^^^^^^^^^^^

As of version 6.4.0, ua-ios-pod supports bitcode by default.
