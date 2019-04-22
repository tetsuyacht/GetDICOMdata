# GetDICOMdata
GetDICOMdata has commands for reading a DICOM file and calculating sample and echo spacings in seconds. We have binary files for Ubuntu16.04 and CentOS6 now. You can get these values by executing as follows.
<ul>
<li>getSampleSpacing DICOMfileNameYouWantToRead.dcm</li>
<li>getEchoSpacing DICOMfileNameYouWantToRead.dcm</li>
</ul>
If you want to output a result and related DICOM tag information to a text file, execulte as follows.
<ul>
<li>getSampleSpacing DICOMfileNameYouWantToRead.dcm TextFileNameYouWantToOutput.txt</li>
<li>getEchoSpacing DICOMfileNameYouWantToRead.dcm TextFileNameYouWantToOutput.txt</li>
</ul>
Use shell scripts in each OS directory if you need to compile C++ files by yourself.
