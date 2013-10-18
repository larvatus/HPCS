
/*!
 *   @file DataSet.hpp
     @brief Declaration of DataSet and DataSetLevelled classes.

     @date 08/2013
     @author Nicholas Tarabelloni  <nicholas.tarabelloni@gmail.com>
     @maintainer Nicholas Tarabelloni  <nicholas.tarabelloni@gmail.com>
*/

#ifndef _DATASET_HPP__
#define _DATASET_HPP__

#pragma GCC diagnostic ignored "-Wenum-compare"
#include <Eigen/Dense>
#pragma GCC diagnostic warning "-Wenum-compare"

#include <source/HPCSDefs.hpp>

namespace HPCS
{

//! @class DataSet This class Implements an object representing a dataset of functional data. 
/*!
 * This class Implements an object representing a dataset of functional data. Data are arranged by rows representing the several samples
 * in the set, while columns represent the different observations of the single sample (i.e. the discrete values of the signal).
 * These objects are constructable in various ways, but basically by specifying the size of the dataSet and setting the values, both from
 * an existing array and by reading an external file.
 */  
class DataSet
{
public:

     //! @name Public Types
     //@{
       
       //! Typedef fot the matrix type (variance or correlation matrix)
       typedef Eigen::Matrix< Real, Eigen::Dynamic, Eigen::Dynamic> matrix_Type;
       
       //! Shared pointer to the matrix type
       typedef boost::shared_ptr< matrix_Type > matrixPtr_Type;
       
       //! Typedef for the current matrix type consituting the data structure
       typedef matrix_Type data_Type;
       
       //! Typedef for the shared_ptr to the data structure
       typedef matrixPtr_Type dataPtr_Type;
        
    //@}
    
    //! @name Constructors & Destructor
    //@{
      
      //! Constructor from the number of samples and the number of points
      /*!
       * This constructor initialize a data structure of size (nbSamples, nbPts) to store a future dataSet.
       * 
       * @param nbSamples is the number of samples (signals) in the dataSet, i.e. the number of rows in the storage.
       * @param nbPts is the number of points defining each sample (signal). It corresponds to the number of columns in the data structure.
       */
      DataSet( const UInt & nbSamples, const UInt & nbPts );
    
      //! Constructor from a raw array, the number of samples and the number of points
      /*!
       * This constructor initialize a data structure of size (nbSample, nbPts) and stores the data specified in the raw array.
       * 
       * @param data is a raw array collecting the data to be stored in the data set.
       * @param nbSamples is the number of samples (signals) in the dataSet, i.e. the number of rows in the storage.
       * @param nbPts is the number of points defining each sample (signal). It corresponds to the number of columns in the data structure.
       */
      DataSet( const Real * data, const UInt & nbSamples, const UInt & nPts );
    
      //! Constructor from a std::vector, the number of samples and the number of points
      /*!
       * This constructor initialize a data structure of size (nbSample, nbPts) and stores the data specified in the std::vector array.
       * 
       * @param data is a std::vector array collecting the data to be stored in the data set.
       * @param nbSamples is the number of samples (signals) in the dataSet, i.e. the number of rows in the storage.
       * @param nbPts is the number of points defining each sample (signal). It corresponds to the number of columns in the data structure.
       */
      DataSet( const std::vector< Real > & data, const UInt & nbSamples, const UInt & nbPts );
 
      //! Constructor from matrix type object
      /*!
       * This constructor initialize this data set from the data object received.
       * 
       * @param data is a matrix type representing the data set from which this object is built.
       */
      DataSet( const data_Type & data );
 
      //! Constructor from a shared pointer to a matrix type object
      /*!
       *  This constructor initialize this data set from the pointer received.
       * 
       * @param dataPtr is a shared pointer to a matrix type object used to build this data set.
       */
      DataSet( const dataPtr_Type & dataPtr );
    
      //! Standard distructor
      virtual ~DataSet();

      //@}
      
      
      //! @name Setters and Getters
      //@{
      
	//! Setter of data in the data set 
	/*!
	 * Setter of data in the data set from a raw array variable
	 * 
	 * @param data is a raw array containing the values defining the dataset
	 * @param nbSamples is the number of samples (i.e. rows) in the dataset
	 * @param nbPts is the number of observations per sample (i.e. the columns) in the dataset.
	 */
	void setData( const Real * data, const UInt & nbSamples, const UInt & nPts);
    
	//! Setter of data in the data set 
	/*!
	 * Setter of data in the data set from a std::vector variable
	 * 
	 * @param data is a std::vector array containing the values defining the dataset
	 * @param nbSamples is the number of samples (i.e. rows) in the dataset
	 * @param nbPts is the number of observations per sample (i.e. the columns) in the dataset.
	 */
	void setData( const std::vector< Real > & data, const UInt & nbSamples, const UInt & nPts);
	
	//! Setter of data in the data set 
	/*!
	 * Setter of data in the data set from a matrix variable.
	 * 
	 * @param data  is a matrix variable containing the data to be stored in this object.
	 */
	void setData( const data_Type & data);

	//! Setter of data in the data set 
	/*!
	 * Setter of data in the data set from a shared pointer to a matrix variable.
	 * 
	 * @param dataPtr is a shared pointer to  a matrix variable containing the data to be stored in this object.
	 */
	void setData( const dataPtr_Type & dataPtr );

	//! Setter of the left and right offsets 
	/*!
	 * Setter of the left and right offsets. Useful when data are to be read from an external file.
	 * 
	 * @param leftOffset is the number of character offsets from the left to be considered when reading data from file.
	 * @param leftOffset is the number of character offsets from the right to be considered when reading data from file.
	 */	
	void setOffset( const UInt & leftOffset, const UInt & rightOffset );
	
	//! Getter of the data structure
	dataPtr_Type getData(){ return this->M_data; };
    
	//! Getter of a row subset
	/*!
	 * It returns a shared pointer to a subset (i.e. a smaller data set) containing the rows specified by the std::vector of IDs.
	 * 
	 * @param IDs is a std::vector containing the IDs of the desired rows to be returned as a subset of the current data set.
	 */
 	dataPtr_Type getRowSubSet( const std::vector< UInt > & IDs ) const;
     
	//! Getter of the number of samples
	UInt nbSamples() const { return this->M_nbSamples; }
    
	//! Getter of the number of points
	UInt nbPts() const { return this->M_nbPts; }
    
      //@}
      
      //! @name Misc. Public Types
      //@{

	//! Method to read data from an external file
	/*!
	 * It reads data from the specified file and stores them inside this object.
	 * 
	 * @param filename is the name of the file containing data to be read.
	 */
	void readData( const std::string & filename );
	
	//! Method to write data from an external file
	/*!
	 * It writes data to a specified destination. Default is std::cout, but also ofstream objects corresponding to
	 * files can be accepted..
	 * 
	 * @param output is the destination to which data must be written.
	 */
	void writeData( std::ostream & output = std::cout ) const;
	
	//! ShowMe method to print to some destination the internal status of this object
	/*!
	 * It writes to some specified destination the current status of this object.
	 * 
	 * @param output is the destination to which info must be written.
	 */
	virtual void showMe( std::ostream & output = std::cout ) const;
	
	//! Method to access data at a specified position
	/*!
	 *  This method allows access data at the specified position, without permission to change them.
	 * 
	 * @param sample desired sample (i.e. row index)
	 * @param pt desider pt (i.e. column index)
	 */
	Real operator()( const UInt & sample, const UInt & pt ) const;
	
      //@}
    
protected:
  
      //! @name Protected Members
      //@{

	//! Shared pointer to a matrix object containing the dataset.
	dataPtr_Type M_data;

	//! Number of samples (i.e. rows)
	UInt M_nbSamples;

	//! Number of observations for each sample (i.e. columns)
	UInt M_nbPts;
	
	//! Number of character offsets from left when reading data from file.
	UInt M_leftOffset;
	
	//! Number of character offsets from right when reading data from file 
	UInt M_rightOffset;
  
    //@}
	
	
private:
  
  
};

//! @class DataSetLevelled This class Implements an object representing a dataset of functional data with different levels. 
/*!
 * This class implements an object representing a dataset of functional data with different levels grouping the samples. Being a derivate class of
 * DataSet, it stores data in a matrix-wise fashion, but in addition it contains information useful to group data into two or more subsets.
 * Separation into groups is the representation of some "abstract" difference among the samples. Although that they all come from the same context and thus
 * the user is likely to process them altogether, for this reason data are still arranged in a single, dense, unpermutated matrix. The only extra information
 * stored here are levels.
 */  
class DataSetLevelled : public DataSet 
{
  
public:
  
  //! @name Public Types
  //@{
 
     //! Typedef to avoid fully qualification of data_Type name
     typedef DataSet::data_Type data_Type;
    
     //! Typedef to avoid fully qualification of dataPtr_Type name
     typedef DataSet::dataPtr_Type dataPtr_Type;
    
     //! Typedef for the structure containing the IDs of a single group
     typedef std::set< UInt > IDContainer_Type;
    
     //! Typedef of the vector containing the several subsets of IDs
     typedef std::vector< IDContainer_Type > levelsContainer_Type;
    
     //! Typedef for a shared pointer to a levels container
     typedef boost::shared_ptr< levelsContainer_Type > levelsContainerPtr_Type;
    
  //@}
  
  //! @name Constructors & Destructor 
  //@{
    
      //! Constructor from the number of samples, the number of points, and the number of levels
      /*!
       *  Constructor from the number of samples, the number of points, and the number of levels
       * 
       * @param nbSmaples is the number of samples in the dataset.
       * @param nbPts is the number of points (i.e.column) for each sample
       * @param nbLevels is the number of levels (i.e. groups) in the dataset
       */
      DataSetLevelled( const UInt & nbSamples, const UInt & nbPts, const UInt & nbLevels );
 
      //! Constructor from the raw data, the number of samples, the number of points, and the number of levels
      /*!
       *  Constructor from the raw data, the number of samples, the number of points, and the number of levels
       * 
       * @param data is the raw array containing data to be stored in this dataset
       * @param nbSmaples is the number of samples in the dataset.
       * @param nbPts is the number of points (i.e.column) for each sample
       * @param nbLevels is the number of levels (i.e. groups) in the dataset
       */
      DataSetLevelled( const Real * data, const UInt & nbSamples, const UInt & nbPts, const UInt & nbLevels );
  
      //! Constructor from the std::vector array of data, the number of samples, the number of points, and the number of levels
      /*!
       *  Constructor from the std::vector array of data, the number of samples, the number of points, and the number of levels
       * 
       * @param data is the std::vector array containing data to be stored in this dataset
       * @param nbSmaples is the number of samples in the dataset.
       * @param nbPts is the number of points (i.e.column) for each sample
       * @param nbLevels is the number of levels (i.e. groups) in the dataset
       */
      DataSetLevelled( const std::vector< Real > & data, const UInt & nbSamples, const UInt & nbPts, const UInt & nbLevels );
  
      //! Constructor from a base DataSet object and the number of levels
      /*!
       *  Constructor from a base DataSet object and the number of levels
       * 
       * @param data is a base DataSet object
       * @param nbLevels is the number of levels (i.e. groups) in the dataset
       */
      DataSetLevelled( const data_Type & data, const UInt & nbLevels );
  
      //! Constructor from a shared pointer to a DataSet object and the number of levels
      /*!
       *  Constructor from a shared pointer to a DataSet object and the number of levels
       * 
       * @param data is a shared pointer to a DataSet object
       * @param nbLevels is the number of levels (i.e. groups) in the dataset
       */
      DataSetLevelled( const dataPtr_Type & dataPtr, const UInt & nbLevels );
  
      //! Standard destructor
      virtual ~DataSetLevelled();
      
  //@}

  //! @name Setters & Getters
  //@{

      //! Setter of the levels
      /*!
       *  Setter of the levels in the dataSet from a specified levelsContainer, i.e. a vector of maps
       *  specifying which IDs belong to which group. Note that, owing to the lack of a-priori information about
       *  the distribution of IDs in groups and the possible need for searches among IDs, a map storing IDs is an
       *  efficient structure. 
       * 
       * @param levelsPtr is a shared_ptr to a levelsContainer object.
       */
      void setLevels( const levelsContainerPtr_Type & levelsPtr );
  
      //! Setter of the levels
      /*!
       *  Setter of the levels in the dataSet from a vector of linear extrema, i.e. a vector of
       *  IDs representing the extrema of the corresponding groups. A division into groups by linear chunks
       *  of IDs is implicitly assumed.
       * 
       * @param linearExtrema is the vector of linear IDs.
       */
      void setLevels( const std::vector< UInt > & linearExtrema );
  
      //! Setter of the levels
      /*!
       *  Setter of the levels in the dataSet from a filename containin the linear extrema to
       *  be considered when building the levels structure.
       * 
       * @param levelsPtr is a shared_ptr to a levelsContainer object.
       */
      void setLevelsFromExtrema( const std::string & levelsExtremaFilename );
      
      //! Getter of the IDs of the specified levelID
      IDContainer_Type & levelIDs( const UInt lev ) const;
  
      //! Getter of the cardinality of the current levelID
      UInt cardinality( const UInt & levelID );

    
  //@}
  
  
  //! @name Misc. Public Methods
  //@{
  
    //! ShowMe method to print to some destination the internal status of this object
    /*!
     * It writes to some specified destination the current status of this object.
     * 
     * @param output is the destination to which info must be written.
     */
    virtual void showMe( std::ostream & output = std::cout ) const;  
    
  //@}
  
  
   
private:
  
  //! @name Private members
  //@{
   
      //! The shared pointer to the levels container
      levelsContainerPtr_Type M_levelsPtr;
  
      //! The number of levels of current data set
      UInt M_nbLevels;
  
      //! The cardinality of each level
      std::vector< UInt > M_cardinality;
  
  //@}
  
};


}

#endif /* _DATASET_HPP__ */

