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

#ifndef OMPL_BASE_MANIFOLDS_REAL_VECTOR_STATE_MANIFOLD_
#define OMPL_BASE_MANIFOLDS_REAL_VECTOR_STATE_MANIFOLD_

#include "ompl/base/StateManifold.h"
#include "ompl/base/manifolds/RealVectorBounds.h"
#include <vector>

namespace ompl
{
    namespace base
    {
	
	/** \brief State sampler for the R<sup>n</sup> manifold */
	class RealVectorStateSampler : public ManifoldStateSampler
	{
	public:
	    
	    RealVectorStateSampler(const StateManifold *manifold) : ManifoldStateSampler(manifold)
	    {
	    }
	    
	    virtual void sampleUniform(State *state);	    
	    virtual void sampleUniformNear(State *state, const State *near, const double distance);
	    virtual void sampleGaussian(State *state, const State *mean, const double stdDev);	    
	};
	
	/** \brief A manifold representing R<sup>n</sup>. The distance function is the L2 norm. */
	class RealVectorStateManifold : public StateManifold
	{
	public:

	    /** \brief The definition of a state in R<sup>n</sup> */
	    class StateType : public State
	    {
	    public:
		StateType(void) : State()
		{
		}
		
		/** \brief Access element i of values.  This does not
		    check whether the index is within bounds */
		double operator[](unsigned int i) const
		{
		    return values[i];
		}
		
		/** \brief Access element i of values.  This does not
		    check whether the index is within bounds */
		double& operator[](unsigned int i)
		{
		    return values[i];
		}
		
		/** \brief The value of the actual vector in R<sup>n</sup> */
		double *values;
	    };
	    
	    RealVectorStateManifold(unsigned int dim) : StateManifold(), dimension_(dim), stateBytes_(dim * sizeof(double)), bounds_(dim)
	    {
	    }
	    
	    virtual ~RealVectorStateManifold(void)
	    {	
	    }
	    
	    /** \brief Set the bounds of this manifold. This defines
		the range of the space in which sampling is
		performed. */
	    void setBounds(const RealVectorBounds &bounds);

	    /** \brief Get the bounds for this manifold */
	    const RealVectorBounds& getBounds(void) const
	    {
		return bounds_;
	    }
	    
	    virtual unsigned int getDimension(void) const;
	    
	    virtual double getMaximumExtent(void) const;

	    virtual void enforceBounds(State *state) const;

	    virtual bool satisfiesBounds(const State *state) const;

	    virtual void copyState(State *destination, const State *source) const;

	    virtual double distance(const State *state1, const State *state2) const;

	    virtual bool equalStates(const State *state1, const State *state2) const;

	    virtual void interpolate(const State *from, const State *to, const double t, State *state) const;

	    virtual ManifoldStateSamplerPtr allocStateSampler(void) const;

	    virtual State* allocState(void) const;
	    
	    virtual void freeState(State *state) const;

	    virtual void printState(const State *state, std::ostream &out) const;

	    virtual void printSettings(std::ostream &out) const;

	    virtual void setup(void);
	    
	protected:
	    
	    unsigned int     dimension_;
	    std::size_t      stateBytes_;
	    RealVectorBounds bounds_;

	};
    }
}

#endif
