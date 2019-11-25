# RSI : robust sketch integration

This repo implement the method to find unique region of a segmented map using the heat kernel distance as presented in the [URSIM method paper](https://www.mdpi.com/2218-6581/8/2/43).

If you use this, please cite the URSIM method:

```
@article{Mielle_2019, 
	title={URSIM: Unique Regions for Sketch Map Interpretation and Matching}, 
	volume={8}, 
	ISSN={2218-6581}, 
	url={http://dx.doi.org/10.3390/robotics8020043}, 
	DOI={10.3390/robotics8020043}, 
	number={2}, 
	journal={Robotics}, 
	publisher={MDPI AG}, 
	author={Mielle, Malcolm and Magnusson, Martin and Lilienthal, Achim}, 
	year={2019}, 
	month={Jun}, 
	pages={43}}
```


# Exec

An overall super needed README

test_pca : compute the zone of a bunch a image in a folder

test_hunagrian : test the hungarian library

test_zone_hungarian_itself : match a graph zone extracted from an image onto itself

test_zone_hungarian : match two grah from different images but from same environment.

# Dependencies


* [Edit distance](https://github.com/MalcolmMielle/edit_distance_cpp)
* [maoris](https://github.com/MalcolmMielle/maoris)
