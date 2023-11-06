# Parallax: Compute Parallax for difference between two images

- Test Pair
  <table>
    <tr>
       <td> Image 1 </td>
       <td> Image 2 </td>
    </tr> 
    <tr>
       <td><img src="./img/cand_1_3.png"/> </td>
       <td><img src="./img/cand_1_35.png"/> </td>
    </tr>
    <tr>
       <td><img src="./img/cand_1_3.png"/> </td>
       <td><img src="./img/cand_1_9.png"/> </td>
    </tr>
  </table>

## 1. Dependencies
   - OpenCV 3.2.0, OpenCV Contrib 3.2.0, C++ 11 version 

## 2. Methodology to apply feature matching
  - Use this Github Page [Feature Matching](https://github.com/SungJaeShin/Feature_matching)

## 3. Results (EuRoC dataset)
  - **[SIFT & SURF CASE]** Parallax Results btw "**cand_1_3**" and "**cand_1_35**" 
      ```
      [Before Filtering]
      max dx: 53.6179, max dy: 27.4187, max dist: 305.486
      avg dx: 16.4145, avg dy: 10.3494, avg dist: 27.3653
      
      [After Filtering]
      max dx: 24.8738, max dy: 20.0451, max dist: 24.9846
      avg dx: 15.0051, avg dy: 16.2881, avg_dist: 22.3998
      
      before filtering correspondence pair: 69
      after filtering correspondence pair: 63
      ```

    - Original Method using [Feature Matching](https://github.com/SungJaeShin/Feature_matching)
      
      <img src="./before_filter.png"/>
    
    - Filtered using parallax method
      
      <img src="./after_filter.png"/>
  
    - Failure case using parallax method
  
      <img src="./failure_case.png"/>

  - **[ORB & SURF CASE]** Parallax Results btw "**cand_1_3**" and "**cand_1_35**" 
      ```
      [Before Filtering]
      max dx: 54.5472, max dy: 336.038, max dist: 340.437
      avg dx: 15.6673, avg dy: 18.3601, avg dist: 24.8476
      
      [After Filtering]
      max dx: 18.72, max dy: 21, max dist: 25.2389
      avg dx: 14.2445, avg dy: 17.2931, avg_dist: 22.4969

      before filtering correspondence pair: 215
      after filtering correspondence pair: 203
      ```

    - Original Method using [Feature Matching](https://github.com/SungJaeShin/Feature_matching)
      
      <img src="./before_filter_orb.png"/>
    
    - Filtered using parallax method
      
      <img src="./after_filter_orb.png"/>
  

