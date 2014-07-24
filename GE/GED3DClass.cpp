#include "GED3DClass.h"

#include "GEManageClass.h"
#include "GETimerClass.h"

namespace GE
{
	D3DClass* g_D3DClass;

	//-----------------------------------------------------------------------
	bool D3DClass::Initialize()
	{
		/*
		m_device = NULL;
		m_deviceContext = NULL;
		m_swapChain = NULL;
		m_renderTargetView = NULL;
		m_depthStencilBuffer = NULL;
		m_depthStencilView = NULL;
		*/

		HRESULT hr;

		/******************************��ȡ��ʾˢ���ʣ����ڴ�ֱͬ��******************************/
		/******************************��ȡ��ʾˢ���ʣ����ڴ�ֱͬ��******************************/
		//���洹ֱͬ���趨
		//��ȡ��ʾˢ���ʣ����ڴ�ֱͬ����
		IDXGIFactory* factory;
		hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
		if(FAILED(hr))
		{
			return false;
		}

		IDXGIAdapter* adapter;
		hr = factory->EnumAdapters(0, &adapter);
		if(FAILED(hr))
		{
			return false;
		}

		IDXGIOutput* adapterOutput;
		hr = adapter->EnumOutputs(0, &adapterOutput);
		if(FAILED(hr))
		{
			return false;
		}

		unsigned int numModes;
		hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
		if(FAILED(hr))
		{
			return false;
		}

		DXGI_MODE_DESC* displayModeList;
		displayModeList = new DXGI_MODE_DESC[numModes];
		if(!displayModeList)
		{
			return false;
		}

		hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
		if(FAILED(hr))
		{
			return false;
		}
		unsigned int mWidth = GetSystemMetrics(SM_CXSCREEN);             //��ȡ�͵�ǰ��Ļ��ƥ��ķֱ���
		unsigned int mHeight = GetSystemMetrics(SM_CYSCREEN);            //��ȡ�͵�ǰ��Ļ��ƥ��ķֱ���
		unsigned int numerator, denominator;                             //���ڱ���͵�ǰ��Ļ�ֱ���ƥ���ˢ����
		for(unsigned int i=0; i<numModes; i++)                           
		{
			if(displayModeList[i].Width == (unsigned int)mWidth
				&& displayModeList[i].Height == (unsigned int)mHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
		/******************************************************************************************/
		/******************************************************************************************/



		/******************************��ȡ�Կ��Դ��Լ��Կ����ͺ�******************************/
		/******************************��ȡ�Կ��Դ��Լ��Կ����ͺ�******************************/
		//��ȡ�Դ�
		DXGI_ADAPTER_DESC adapterDesc;
		hr = adapter->GetDesc(&adapterDesc);
		if(FAILED(hr))
		{
			return false;
		}

		m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

		int error;
		unsigned int stringLength;
		error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
		if(error != 0)
		{
			return false;
		}
		MessageBoxA(NULL, m_videoCardDescription, "��ǰ�Կ��ͺ�",MB_OK);
		delete [] displayModeList;
		displayModeList = NULL;
		adapterOutput->Release();
		adapterOutput = NULL;
		adapter->Release();
		adapter = NULL;
		factory->Release();
		factory = NULL;
		/******************************************************************************************/
		/******************************************************************************************/



		/******************************����D3D�豸��D3D�豸������******************************/
		/******************************����D3D�豸��D3D�豸������******************************/
		//����D3D�豸��D3D�豸������
		D3D_FEATURE_LEVEL featureLevels[6] = {	
			D3D_FEATURE_LEVEL_11_0,        //���Եȼ�
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};
		D3D_FEATURE_LEVEL curLevel;

		hr = D3D11CreateDevice(
			NULL,                            //�Կ�ָ��,NULLʹ��Ĭ�ϵ�������
			D3D_DRIVER_TYPE_HARDWARE,        //��������,��Ӳ����֧��ʱ����ʹ��D3D_DRIVER_TYPE_REFERENCE
			NULL,                            //software driver��DLL�е�һ�������դ��������ʹ��Ӳ��������ΪNULL
			0,                               //D3D_CREATE_DEVICE_FLAG
			featureLevels,                   //ָ��һ��D3D_FEATURE_LEVEL�����ָ��
			6,                               //pFeatureLevels�е�Ԫ������
			D3D11_SDK_VERSION,               //SDK�汾��ʹ��D3D11_SDK_VERSION
			&m_device,                       //����һ�������õ�D3D�豸�����ָ���ַ
			&curLevel,                       //����һ��D3D_FEATURE_LEVELָ��
			&m_deviceContext);               //����һ��D3D�豸�����Ķ���ָ��
		if(FAILED(hr))
		{
			return false;
		}
		if(curLevel != D3D_FEATURE_LEVEL_11_0)
		{
			if(IDNO == MessageBox(NULL,L"���Ļ�����֧��D3D11���ԣ���������޷���ȷ��ʾЧ����Ҫ������",L"����",MB_YESNO))
			{
				return false;
			}
		}
		/******************************************************************************************/
		/******************************************************************************************/



		/******************************�����������Լ���ȾĿ����ͼ******************************/
		/******************************�����������Լ���ȾĿ����ͼ******************************/
		//��ȡ���ز���֧�ֵȼ�
		m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM,4,&m_x4MultiQuality);
		//��д����������
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferDesc.Width = m_width;                          //���������
		swapChainDesc.BufferDesc.Height = m_height;                        //�������߶�
		if( m_isVsync )                                                    //ˢ���ʣ�������Ƿ�ֱͬ�����в�ͬ������
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
		}
		else
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 70;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		}
		swapChainDesc.BufferDesc.Format           = DXGI_FORMAT_R8G8B8A8_UNORM;             //���������ݸ�ʽ
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling          = DXGI_MODE_SCALING_UNSPECIFIED;
		if( m_isSSAA )
		{
			swapChainDesc.SampleDesc.Count            = 4;                                      //���ز���������ݣ���ÿ�����صĲ��������
			swapChainDesc.SampleDesc.Quality          = m_x4MultiQuality-1;                     //�����ȼ���0�ȼ���ʾ��ʹ�ö��ز���
		}
		else
		{
			swapChainDesc.SampleDesc.Count            = 1;                                      //���ز���������ݣ���ÿ�����صĲ��������
			swapChainDesc.SampleDesc.Quality          = 0;                                      //�����ȼ���0�ȼ���ʾ��ʹ�ö��ز���
		}
		swapChainDesc.BufferUsage                 = DXGI_USAGE_RENDER_TARGET_OUTPUT;        //Usage
		swapChainDesc.BufferCount                 = 1;                                      //����������  
		swapChainDesc.OutputWindow                = m_hWnd;                                 //�����ھ��
		if( m_isFullScreen )                                                  //�Ƿ񴰿ڻ�
		{
			swapChainDesc.Windowed = false;
		}
		else
		{
			swapChainDesc.Windowed = true;
		}
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;                    //ö���ͣ�������ʽ
		swapChainDesc.Flags = 0;                                                //ö���ͣ�swap-chain behavior
		//����������
		IDXGIDevice *pDxgiDevice(NULL);
		hr = m_device->QueryInterface(__uuidof(IDXGIDevice),reinterpret_cast<void**>(&pDxgiDevice));
		if(FAILED(hr))
		{
			MessageBox(NULL, L"Get DXGIDevice failed!", L"ERROR",MB_OK);
			return false;
		}
		IDXGIAdapter *pDxgiAdapter(NULL);
		hr = pDxgiDevice->GetParent(__uuidof(IDXGIAdapter),reinterpret_cast<void**>(&pDxgiAdapter));
		if(FAILED(hr))
		{
			MessageBox(NULL, L"Get DXGIAdapter failed!", L"ERROR",MB_OK);
			return false;
		}
		IDXGIFactory *pDxgiFactory(NULL);
		hr = pDxgiAdapter->GetParent(__uuidof(IDXGIFactory),reinterpret_cast<void**>(&pDxgiFactory));
		if(FAILED(hr))
		{
			MessageBox(NULL, L"Get DXGIFactory failed!", L"ERROR",MB_OK);
			return false;
		}
		hr = pDxgiFactory->CreateSwapChain(m_device,                     //ָ��һ�������õ�D3D�豸�����ָ���ַ
			&swapChainDesc,               //ָ��һ��������������ָ��
			&m_swapChain);                //����һ�������������ָ���ַ�����������������Ⱦ
		if(FAILED(hr))
		{
			MessageBox(NULL, L"Create swap chain failed!", L"ERROR",MB_OK);
			return false;
		}
		pDxgiFactory->Release();
		pDxgiFactory = NULL;
		pDxgiAdapter->Release();
		pDxgiAdapter = NULL;
		pDxgiDevice->Release();
		pDxgiDevice = NULL;

		//������ȾĿ����ͼ(�����û���󶨵���ȾĿ����ͼ��)
		ID3D11Texture2D* backBufferPtr;
		hr = m_swapChain->GetBuffer(0,                            //���û����������һ��ֻ��һ�����û���
			__uuidof(ID3D11Texture2D),    //����ӿ�����
			(LPVOID*)&backBufferPtr);     //��ý������󻺴��ַ�������backBufferPtr
		if(FAILED(hr))
		{
			return false;
		}

		hr = m_device->CreateRenderTargetView(backBufferPtr,                                    //��ͼ��Ӧ��Դ
			NULL,                                             //��ͼ��������ǰ�Ѿ������˺��û������������
			&m_renderTargetView);                             //Ҫ��������ͼ��ָ��ĵ�ַ��
		if(FAILED(hr))
		{
			return false;
		}
		backBufferPtr->Release(); 
		backBufferPtr = 0;
		/******************************************************************************************/
		/******************************************************************************************/



		/******************************�������/ģ�滺�棬���/ģ����ͼ******************************/
		/******************************�������/ģ�滺�棬���/ģ����ͼ******************************/
		//��д���ģ�滺������
		D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
		ZeroMemory(&depthStencilBufferDesc, sizeof(depthStencilBufferDesc));
		depthStencilBufferDesc.Width                = m_width;             //��Ȼ���Ŀ��
		depthStencilBufferDesc.Height               = m_height;            //��Ȼ���ĸ߶�
		depthStencilBufferDesc.MipLevels            = 1;                                 //��ͼϸ��
		depthStencilBufferDesc.ArraySize            = 1;                                 //��������
		depthStencilBufferDesc.Format               = DXGI_FORMAT_D24_UNORM_S8_UINT;     //���ݸ�ʽ��һ��ΪDXGI_FORMAT_D24_UNORM_S8_UINT,24λ������ȣ�8λ����ģ��  
		if( m_isSSAA )
		{
			depthStencilBufferDesc.SampleDesc.Count     = 4;                                 //���ز�����������ǰ���һ����
			depthStencilBufferDesc.SampleDesc.Quality   = m_x4MultiQuality-1;                //���ز�����������ǰ���һ����
		}
		else
		{
			depthStencilBufferDesc.SampleDesc.Count     = 1;                                 //���ز�����������ǰ���һ����
			depthStencilBufferDesc.SampleDesc.Quality   = 0;                                 //���ز�����������ǰ���һ����
		}
		depthStencilBufferDesc.Usage                = D3D11_USAGE_DEFAULT;               //����DEFAULTΪֻ��GPU��д��Դ ��������CPU��д
		depthStencilBufferDesc.BindFlags            = D3D11_BIND_DEPTH_STENCIL;          //�����ͣ������������/ģ�滺��
		depthStencilBufferDesc.CPUAccessFlags       = 0;                                 //��0ΪCPU���ɷ��ʣ����/ģ�滺��ֻ��ҪGPU��д�Ϳ�����
		depthStencilBufferDesc.MiscFlags            = 0;                                 //������������ﲻ�����������/ģ�滺��
		//�������ģ�建��
		hr = m_device->CreateTexture2D(&depthStencilBufferDesc, NULL, &m_depthStencilBuffer);
		if( FAILED(hr) )
			return false;

		//��ʼ�����ģ����ͼ��������
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;     //���ʹ���˶��ز������Ҫʹ���ض��꣨�˲���û��ʹ�ã�Ԥ����
		depthStencilViewDesc.Flags = 0;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		//�������ģ����ͼ
		hr = m_device->CreateDepthStencilView(m_depthStencilBuffer, NULL, &m_depthStencilView);
		if( FAILED(hr) )
			return false;
		/******************************************************************************************/
		/******************************************************************************************/



		/******************************����ȾĿ����ͼ�����ģ����ͼ�󶨵���Ⱦ����******************************/
		/******************************����ȾĿ����ͼ�����ģ����ͼ�󶨵���Ⱦ����******************************/
		//����ȾĿ����ͼ�����ģ����ͼ�󶨵���Ⱦ����(Output Merger Stage!!!)
		m_deviceContext->OMSetRenderTargets(1,                                //RenderTarget����
			&m_renderTargetView,              //RenderTarget����
			m_depthStencilView);              //DepthStencil view
		/******************************************************************************************/
		/******************************************************************************************/



		/******************************�����ӿ�******************************/
		/******************************�����ӿ�******************************/
		//�����ӿ���Ϣ
		D3D11_VIEWPORT viewport;
		viewport.TopLeftX  = 0.0f;                                //�ӿ����Ͻ�����Ļ��x����
		viewport.TopLeftY  = 0.0f;                                //�ӿ����Ͻ�����Ļ��y����
		viewport.Width     = (float)m_width;                      //�ӿڿ��
		viewport.Height    = (float)m_height;                     //�ӿڸ߶�
		viewport.MinDepth  = 0.0f;                                //��С���ֵ��0.0f
		viewport.MaxDepth  = 1.0f;                                //������ֵ��1.0f  

		//�����ӿ�
		m_deviceContext->RSSetViewports(1,               //Ҫ�󶨵��ӿ���������������Ļ����������˫����Ϸģʽ�ȣ�
			&viewport);      //�ӿ�����
		/******************************************************************************************/
		/******************************************************************************************/

		return true;
	}

	//-----------------------------------------------------------------------
	void D3DClass::Shutdown()
	{
		//m_swapChain->SetFullscreenState(false, NULL);

		SAFE_RELEASE( m_depthStencilView );
		SAFE_RELEASE( m_depthStencilBuffer );
		SAFE_RELEASE( m_renderTargetView );
		SAFE_RELEASE( m_deviceContext );
		SAFE_RELEASE( m_device );
		SAFE_RELEASE( m_swapChain );

		return;
	}

	//-----------------------------------------------------------------------
	void D3DClass::BeginScene(float red, float green, float blue, float alpha)
	{
		float color[4];

		color[0] = red;
		color[1] = green;
		color[2] = blue;
		color[3] = alpha;

		m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
		m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

		return;
	}

	//-----------------------------------------------------------------------
	void D3DClass::BeginScene(const FLOAT4 &_float4)
	{
		float color[4];

		color[0] = _float4.x;
		color[1] = _float4.y;
		color[2] = _float4.z;
		color[3] = _float4.w;

		m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
		m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

		return;
	}

	//-----------------------------------------------------------------------
	void D3DClass::EndScene()
	{
		if( m_isVsync )
		{
			m_swapChain->Present(1, 0);
		}
		else
		{
			m_swapChain->Present(0, 0);
		}

		return;
	}

	//-----------------------------------------------------------------------
	void D3DClass::GetVideoCardInfo(char* cardName, int& memory)
	{
		strcpy_s(cardName, 128, m_videoCardDescription);
		memory = m_videoCardMemory;
		return;
	}

	//-----------------------------------------------------------------------
	void GEBeginScene(float red, float green, float blue, float alpha) { g_D3DClass->BeginScene(red, green, blue, alpha); }

	//-----------------------------------------------------------------------
	void GEBeginScene(const FLOAT4 &_float4) { g_D3DClass->BeginScene(_float4); }

	//-----------------------------------------------------------------------
	void GEEndScene() { g_D3DClass->EndScene(); }
}