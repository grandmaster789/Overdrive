namespace gli{
namespace detail
{
	template <typename vec_type>
	struct compute_transform_1d
	{
		typedef typename transform_func<vec_type>::type func_type;
		typedef texture1D::size_type size_type;
		typedef texture1D::texelcoord_type texelcoord_type;
		
		static void call(texture1D & Output, texture1D const & A, texture1D const & B, func_type Func)
		{
			GLI_ASSERT(all(equal(A.dimensions(), B.dimensions())));
			GLI_ASSERT(A.levels() == B.levels());
			GLI_ASSERT(A.size() == B.size());
			
			for(size_type LevelIndex = 0, LevelCount = A.levels(); LevelIndex < LevelCount; ++LevelIndex)
			{
				texelcoord_type const TexelCount(A.dimensions(LevelIndex));
				texelcoord_type TexelIndex(0);
				
				for(TexelIndex.x = 0; TexelIndex.x < TexelCount.x; ++TexelIndex.x)
				{
					Output.store<vec_type>(TexelIndex, LevelIndex, Func(
						A.load<vec_type>(TexelIndex, LevelIndex),
						B.load<vec_type>(TexelIndex, LevelIndex)));
				}
			}
		}
	};
	
	template <typename vec_type>
	struct compute_transform_1d_array
	{
		typedef typename transform_func<vec_type>::type func_type;
		typedef texture1DArray::size_type size_type;
		typedef texture1DArray::texelcoord_type texelcoord_type;
		
		static void call(texture1DArray & Output, texture1DArray const & A, texture1DArray const & B, func_type Func)
		{
			GLI_ASSERT(all(equal(A.dimensions(), B.dimensions())));
			GLI_ASSERT(A.layers() == B.layers());
			GLI_ASSERT(A.levels() == B.levels());
			GLI_ASSERT(A.size() == B.size());
			
			for(size_type LayerIndex = 0, LayerCount = A.layers(); LayerIndex < LayerCount; ++LayerIndex)
			for(size_type LevelIndex = 0, LevelCount = A.levels(); LevelIndex < LevelCount; ++LevelIndex)
			{
				texelcoord_type const TexelCount(A.dimensions(LevelIndex));
				texelcoord_type TexelIndex(0);
				
				for(TexelIndex.x = 0; TexelIndex.x < TexelCount.x; ++TexelIndex.x)
				{
					Output.store<vec_type>(TexelIndex, LayerIndex, LevelIndex, Func(
						A.load<vec_type>(TexelIndex, LayerIndex, LevelIndex),
						B.load<vec_type>(TexelIndex, LayerIndex, LevelIndex)));
				}
			}
		}
	};
	
	template <typename vec_type>
	struct compute_transform_2d
	{
		typedef typename transform_func<vec_type>::type func_type;
		typedef texture2D::size_type size_type;
		typedef texture2D::texelcoord_type texelcoord_type;
			
		static void call(texture2D & Output, texture2D const & A, texture2D const & B, func_type Func)
		{
			GLI_ASSERT(all(equal(A.dimensions(), B.dimensions())));
			GLI_ASSERT(A.levels() == B.levels());
			GLI_ASSERT(A.size() == B.size());
				
			for(size_type LevelIndex = 0, LevelCount = A.levels(); LevelIndex < LevelCount; ++LevelIndex)
			{
				texelcoord_type const TexelCount(A.dimensions(LevelIndex));
				texelcoord_type TexelIndex(0);
				
				for(TexelIndex.y = 0; TexelIndex.y < TexelCount.y; ++TexelIndex.y)
				for(TexelIndex.x = 0; TexelIndex.x < TexelCount.x; ++TexelIndex.x)
				{
					Output.store<vec_type>(TexelIndex, LevelIndex, Func(
						A.load<vec_type>(TexelIndex, LevelIndex),
						B.load<vec_type>(TexelIndex, LevelIndex)));
				}
			}
		}
	};
		
	template <typename vec_type>
	struct compute_transform_2d_array
	{
		typedef typename transform_func<vec_type>::type func_type;
		typedef texture2DArray::size_type size_type;
		typedef texture2DArray::texelcoord_type texelcoord_type;
		
		static void call(texture2DArray & Output, texture2DArray const & A, texture2DArray const & B, func_type Func)
		{
			GLI_ASSERT(all(equal(A.dimensions(), B.dimensions())));
			GLI_ASSERT(A.layers() == B.layers());
			GLI_ASSERT(A.levels() == B.levels());
			GLI_ASSERT(A.size() == B.size());
				
			for(size_type LayerIndex = 0, LayerCount = A.layers(); LayerIndex < LayerCount; ++LayerIndex)
			for(size_type LevelIndex = 0, LevelCount = A.levels(); LevelIndex < LevelCount; ++LevelIndex)
			{
				texelcoord_type const TexelCount(A.dimensions(LevelIndex));
				texelcoord_type TexelIndex(0);
				
				for(TexelIndex.y = 0; TexelIndex.y < TexelCount.y; ++TexelIndex.y)
				for(TexelIndex.x = 0; TexelIndex.x < TexelCount.x; ++TexelIndex.x)
				{
					Output.store<vec_type>(TexelIndex, LayerIndex, LevelIndex, Func(
						A.load<vec_type>(TexelIndex, LayerIndex, LevelIndex),
						B.load<vec_type>(TexelIndex, LayerIndex, LevelIndex)));
				}
			}
		}
	};

	template <typename vec_type>
	struct compute_transform_3d
	{
		typedef typename transform_func<vec_type>::type func_type;
		typedef texture3D::size_type size_type;
		typedef texture3D::texelcoord_type texelcoord_type;
	
		static void call(texture3D & Output, texture3D const & A, texture3D const & B, func_type Func)
		{
			GLI_ASSERT(all(equal(A.dimensions(), B.dimensions())));
			GLI_ASSERT(A.levels() == B.levels());
			GLI_ASSERT(A.size() == B.size());
		
			for(size_type LevelIndex = 0, LevelCount = A.levels(); LevelIndex < LevelCount; ++LevelIndex)
			{
				texelcoord_type const TexelCount(A.dimensions(LevelIndex));
				texelcoord_type TexelIndex(0);
				
				for(TexelIndex.z = 0; TexelIndex.z < TexelCount.z; ++TexelIndex.z)
				for(TexelIndex.y = 0; TexelIndex.y < TexelCount.y; ++TexelIndex.y)
				for(TexelIndex.x = 0; TexelIndex.x < TexelCount.x; ++TexelIndex.x)
				{
					Output.store<vec_type>(TexelIndex, LevelIndex, Func(
						A.load<vec_type>(TexelIndex, LevelIndex),
						B.load<vec_type>(TexelIndex, LevelIndex)));
				}
			}
		}
	};

	template <typename vec_type>
	struct compute_transform_cube
	{
		typedef typename transform_func<vec_type>::type func_type;
		typedef textureCube::size_type size_type;
		typedef textureCube::texelcoord_type texelcoord_type;
			
		static void call(textureCube & Output, textureCube const & A, textureCube const & B, func_type Func)
		{
			GLI_ASSERT(all(equal(A.dimensions(), B.dimensions())));
			GLI_ASSERT(A.faces() == B.faces());
			GLI_ASSERT(A.levels() == B.levels());
			GLI_ASSERT(A.size() == B.size());
				
			for(size_type FaceIndex = 0, FaceCount = A.faces(); FaceIndex < FaceCount; ++FaceIndex)
			for(size_type LevelIndex = 0, LevelCount = A.levels(); LevelIndex < LevelCount; ++LevelIndex)
			{
				texelcoord_type const TexelCount(A.dimensions(LevelIndex));
				texelcoord_type TexelIndex(0);
				
				for(TexelIndex.y = 0; TexelIndex.y < TexelCount.y; ++TexelIndex.y)
				for(TexelIndex.x = 0; TexelIndex.x < TexelCount.x; ++TexelIndex.x)
				{
					Output.store<vec_type>(TexelIndex, FaceIndex, LevelIndex, Func(
						A.load<vec_type>(TexelIndex, FaceIndex, LevelIndex),
						B.load<vec_type>(TexelIndex, FaceIndex, LevelIndex)));
				}
			}
		}
	};
		
	template <typename vec_type>
	struct compute_transform_cube_array
	{
		typedef typename transform_func<vec_type>::type func_type;
		typedef textureCubeArray::size_type size_type;
		typedef textureCubeArray::texelcoord_type texelcoord_type;
			
		static void call(textureCubeArray & Output, textureCubeArray const & A, textureCubeArray const & B, func_type Func)
		{
			GLI_ASSERT(all(equal(A.dimensions(), B.dimensions())));
			GLI_ASSERT(A.layers() == B.layers());
			GLI_ASSERT(A.levels() == B.levels());
			GLI_ASSERT(A.size() == B.size());
				
			for(size_type LayerIndex = 0, LayerCount = A.layers(); LayerIndex < LayerCount; ++LayerIndex)
			for(size_type FaceIndex = 0, FaceCount = A.faces(); FaceIndex < FaceCount; ++FaceIndex)
			for(size_type LevelIndex = 0, LevelCount = A.levels(); LevelIndex < LevelCount; ++LevelIndex)
			{
				texelcoord_type const TexelCount(A.dimensions(LevelIndex));
				texelcoord_type TexelIndex(0);
				
				for(TexelIndex.y = 0; TexelIndex.y < TexelCount.y; ++TexelIndex.y)
				for(TexelIndex.x = 0; TexelIndex.x < TexelCount.x; ++TexelIndex.x)
				{
					Output.store<vec_type>(TexelIndex, LayerIndex, FaceIndex, LevelIndex, Func(
						A.load<vec_type>(TexelIndex, LayerIndex, FaceIndex, LevelIndex),
						B.load<vec_type>(TexelIndex, LayerIndex, FaceIndex, LevelIndex)));
				}
			}
		}
	};
}//namepsace detail
	
	template <typename vec_type>
	inline void transform(texture1D & Out, texture1D const & In0, texture1D const & In1, typename transform_func<vec_type>::type Func)
	{
		detail::compute_transform_1d<vec_type>::call(Out, In0, In1, Func);
	}
	
	template <typename vec_type>
	inline void transform(texture1DArray & Out, texture1DArray const & In0, texture1DArray const & In1, typename transform_func<vec_type>::type Func)
	{
		detail::compute_transform_1d_array<vec_type>::call(Out, In0, In1, Func);
	}
	
	template <typename vec_type>
	inline void transform(texture2D & Out, texture2D const & In0, texture2D const & In1, typename transform_func<vec_type>::type Func)
	{
		detail::compute_transform_2d<vec_type>::call(Out, In0, In1, Func);
	}
	
	template <typename vec_type>
	inline void transform(texture2DArray & Out, texture2DArray const & In0, texture2DArray const & In1, typename transform_func<vec_type>::type Func)
	{
		detail::compute_transform_2d_array<vec_type>::call(Out, In0, In1, Func);
	}
	
	template <typename vec_type>
	inline void transform(texture3D & Out, texture3D const & In0, texture3D const & In1, typename transform_func<vec_type>::type Func)
	{
		detail::compute_transform_3d<vec_type>::call(Out, In0, In1, Func);
	}
	
	template <typename vec_type>
	inline void transform(textureCube & Out, textureCube const & In0, textureCube const & In1, typename transform_func<vec_type>::type Func)
	{
		detail::compute_transform_cube<vec_type>::call(Out, In0, In1, Func);
	}
	
	template <typename vec_type>
	inline void transform(textureCubeArray & Out, textureCubeArray const & In0, textureCubeArray const & In1, typename transform_func<vec_type>::type Func)
	{
		detail::compute_transform_cube_array<vec_type>::call(Out, In0, In1, Func);
	}
}//namespace gli
