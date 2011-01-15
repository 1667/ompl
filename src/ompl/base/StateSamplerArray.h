/*********************************************************************
* Software License Agreement (BSD License)
* 
*  Copyright (c) 2010, Rice University
*  All rights reserved.
* 
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
* 
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the Rice University nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
* 
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*********************************************************************/

/* Author: Ioan Sucan */

#ifndef OMPL_BASE_STATE_SAMPLER_ARRAY_
#define OMPL_BASE_STATE_SAMPLER_ARRAY_

#include "ompl/base/SpaceInformation.h"
#include "ompl/base/ManifoldStateSampler.h"
#include "ompl/base/ValidStateSampler.h"
#include <vector>
   
namespace ompl
{
    namespace base
    {
	
	/** \brief The type of state samplers we can allocate */
	enum SamplerType
	{
	    /// Allocate a sampler from the manifold contained by the space information (ompl::base::ManifoldStateSampler)
	    SAMPLER_MANIFOLD, 
	    
	    /// Allocate a valid state sampler from the space information (ompl::base::ValidStateSampler)
	    SAMPLER_VALID
	};
	
	/** \brief Depending on the type of sampler, we have different allocation routines

	    This struct will provide that allocation routine,
	    depending on the template argument of ompl::base::SamplerType.*/
	template<SamplerType T>
	struct SamplerSelector
	{
	};

	/** \cond IGNORE */
	template<>
	struct SamplerSelector<SAMPLER_MANIFOLD>
	{
	    typedef ManifoldStateSampler    StateSampler;
	    typedef ManifoldStateSamplerPtr StateSamplerPtr;
	    
	    StateSamplerPtr allocStateSampler(const SpaceInformation *si)
	    {
		return si->allocManifoldStateSampler();
	    }
	    
	};
	
	template<>
	struct SamplerSelector<SAMPLER_VALID>
	{
	    typedef ValidStateSampler    StateSampler;
	    typedef ValidStateSamplerPtr StateSamplerPtr;
	    
	    StateSamplerPtr allocStateSampler(const SpaceInformation *si)
	    {
		return si->allocValidStateSampler();
	    }
	};
	/** \endcond */
	
	/** \brief Class to ease the creation of a set of samplers. This is especially useful for multi-threaded planners. */
	template<SamplerType T>
	class StateSamplerArray 
	{
	public:
	    
	    /** \brief Pointer to the type of sampler allocated */
	    typedef typename SamplerSelector<T>::StateSamplerPtr StateSamplerPtr;

	    /** \brief The type of sampler allocated */
	    typedef typename SamplerSelector<T>::StateSampler    StateSampler;
	    
	    /** \brief Constructor */
	    StateSamplerArray(const SpaceInformationPtr &si) : si_(si.get())
	    {
	    }

	    /** \brief Constructor */
	    StateSamplerArray(const SpaceInformation *si) : si_(si)
	    {
	    }
	    
	    ~StateSamplerArray(void)
	    {
	    }
	    
	    /** \brief Access operator for a specific sampler. For
		performance reasons, the bounds are not checked. */
	    StateSampler* operator[](std::size_t index)
	    {
		return samplers_[index].get();
	    }

	    /** \brief Create or release some state samplers */
	    void resize(std::size_t count)
	    {
		if (samplers_.size() > count)
		    samplers_.resize(count);
		else
		    if (samplers_.size() < count)
		    {
			std::size_t c = samplers_.size();
			samplers_.resize(count);
			for (std::size_t i = c ; i < count ; ++i)
			    samplers_[i] = ss_.allocStateSampler(si_);
		    }
	    }
	    
	    /** \brief Get the count of samplers currently available */
	    std::size_t size(void) const
	    {
		return samplers_.size();
	    }
	    
	private:
	    
	    const SpaceInformation       *si_;
	    SamplerSelector<T>            ss_;
	    std::vector<StateSamplerPtr>  samplers_;
	    
	};
    }
}

#endif
