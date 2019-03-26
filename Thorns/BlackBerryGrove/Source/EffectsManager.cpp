// Hugh Smith 
// copyright 2015
// all rights reserved
#include "EffectsManager.h"


EffectsManager::EffectsManager()
{
}

void EffectsManager::init()
{
    createBlendStates();
    createRasterStates();
    createDepthStencilStates();
    createSamplerStates();
    createConstantBuffers();
 
}

void EffectsManager::Cleanup( void )
{
	for (auto vs : vertexShadersMap)
	{
		if (vs.second->shaderData)
			vs.second->shaderData->Release();
		if (vs.second->vtxShader)
			vs.second->vtxShader->Release();

		delete vs.second;
	}

	for (auto ps : pixelShadersMap)
	{
		if (ps.second->shaderData)
			ps.second->shaderData->Release();
		if (ps.second->pxlShader)
			ps.second->pxlShader->Release();

		delete ps.second;
	}

	for (auto bs : blendStatesMap)
	{
		if (bs.second->blendState)
			bs.second->blendState->Release();

		delete bs.second;
	}

	for (auto rs : rasterStatesMap)
	{
		if (rs.second->rasterState)
			rs.second->rasterState->Release();

		delete rs.second;
	}

	for (auto dss : depthStencilStatesMap)
	{
		if (dss.second->depthStencilState)
			dss.second->depthStencilState->Release();

		delete dss.second;
	}

	for (auto cb : constantBuffersMap)
	{
		if (cb.second->pBff)
			cb.second->pBff->Release();

		delete cb.second;
	}

	for (auto ss : samplerStatesMap)
	{
		if (ss.second->samplerState)
			ss.second->samplerState->Release();

		delete ss.second;
	}

	for (auto il : inputLayoutsMap)
	{
		if (il.second->IPLO)
			il.second->IPLO->Release();

		delete[] il.second;
	}

}

EffectsManager::~EffectsManager()
{

}

bool  EffectsManager::readEDF( wstring Path , edfFileItem &edfFI )
{
    FILE *effectDefFile = NULL;
    TCHAR fileItem[256];
    effectDefFile = _wfopen( Path.c_str(), L"r");
 	if ( !effectDefFile )
	{
 		MessageBox( NULL,
			L"Error opening model definition file - bailing out!", L"Error", MB_OK );
        return false;
    }
    wstring edf = Path.substr(Path.rfind(L"\\")+1);
	Path = Path.substr(0, Path.rfind(L"\\")+1);
	
    if ( edf.length() > 255 ) 
    {
 		MessageBox( NULL,
			L"Your effect definition file is greater than 255 characters - bailing out!", L"Error", MB_OK );
        return false;

    }

    edfFI.path = Path + edf;
   
    fwscanf( effectDefFile, L"VertexShader		    %s\n", &fileItem );
    edfFI.VSFile = wstring(fileItem);
    edfFI.VSFilePath = Path + edfFI.VSFile;
	fwscanf(effectDefFile, L"VSEntryPoint          %s\n", &fileItem);
	edfFI.vsEntryPoint = wstring(fileItem);
    edfFI.cVsEntryPoint = string(edfFI.vsEntryPoint.begin(), edfFI.vsEntryPoint.end());
    fwscanf( effectDefFile, L"VSConstantBuffer	    %s\n", &fileItem);
	edfFI.vsConstantBuffer = wstring(fileItem);
    fwscanf( effectDefFile, L"PixelShader		    %s\n", &fileItem );
    edfFI.PSFile = wstring(fileItem);
    edfFI.PSFilePath = Path + edfFI.PSFile;
	fwscanf(effectDefFile, L"PSEntryPoint		    %s\n", &fileItem);
	edfFI.psEntryPoint = wstring(fileItem);

    edfFI.cPsEntryPoint = string(edfFI.psEntryPoint.begin(), edfFI.psEntryPoint.end());
    fwscanf( effectDefFile, L"PSConstantBuffer		%s\n", &fileItem); 
	edfFI.psConstantBuffer = wstring(fileItem);
    fwscanf( effectDefFile, L"BlendState		    %s\n", &fileItem);
	edfFI.blendState = wstring(fileItem);
    fwscanf( effectDefFile, L"RasterizerState	    %s\n", &fileItem);
	edfFI.rasterState = wstring(fileItem);
    fwscanf( effectDefFile, L"DepthStencilState	    %s\n", &fileItem);
	edfFI.depthStencilState = wstring(fileItem);
    fwscanf( effectDefFile, L"SamplerState	        %s\n", &fileItem);
	edfFI.samplerState = wstring(fileItem);
    fwscanf( effectDefFile, L"InputLayout		    %s\n", &fileItem);
	edfFI.ILO = wstring(fileItem);
    fwscanf( effectDefFile, L"EffectMask            %x\n", &edfFI.EffectMask);
   
}

void EffectsManager::createSamplerStates()
{
    SamplerStateItem * ssi = new SamplerStateItem();
    ssi->samplerState = NULL;
    ZeroMemory( &ssi->samplerDesc, sizeof( D3D11_SAMPLER_DESC ) );
    ssi->samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    ssi->samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    ssi->samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    ssi->samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    ssi->samplerDesc.MipLODBias = 0.0f;
    ssi->samplerDesc.MaxAnisotropy = 16;
    ssi->samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    ssi->samplerDesc.MinLOD = 0.0f;
    ssi->samplerDesc.MaxLOD = FLT_MAX;
    g_pd3dDevice->CreateSamplerState( &ssi->samplerDesc, &ssi->samplerState );
    ssi->samplerName= wstring(L"simpleSampler" );
	samplerStatesMap.insert(std::pair<wstring, SamplerStateItem*>(ssi->samplerName, ssi));
    ssi = new SamplerStateItem();
    ssi->samplerState = NULL;
    ZeroMemory( &ssi->samplerDesc, sizeof( D3D11_SAMPLER_DESC ) );
    ssi->samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    ssi->samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
    ssi->samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
    ssi->samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
    ssi->samplerDesc.MipLODBias = 0.0f;
    ssi->samplerDesc.MaxAnisotropy = 16;
    ssi->samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
    ssi->samplerDesc.MinLOD = 0.0f;
    ssi->samplerDesc.MaxLOD = FLT_MAX;
    g_pd3dDevice->CreateSamplerState( &ssi->samplerDesc, &ssi->samplerState );
    ssi->samplerName = wstring(L"shadowSampler" );
	samplerStatesMap.insert(std::pair<wstring, SamplerStateItem*>(ssi->samplerName, ssi));

    return;
}

void EffectsManager::createBlendStates()
{
    BlendStateItem * bsi = new BlendStateItem();
    bsi->blendState = NULL;
    ZeroMemory( &bsi->blendDesc, sizeof( D3D11_BLEND_DESC ) );
    bsi->blendDesc.IndependentBlendEnable = FALSE;
    bsi->blendDesc.AlphaToCoverageEnable = FALSE;
    bsi->blendDesc.RenderTarget[0].BlendEnable = FALSE;
    bsi->blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    bsi->blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
    bsi->blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    bsi->blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bsi->blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    bsi->blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bsi->blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    g_pd3dDevice->CreateBlendState( &bsi->blendDesc, &bsi->blendState );
    bsi->stateName = wstring(L"NoBlending" );
	blendStatesMap.insert(std::pair<wstring, BlendStateItem*>(bsi->stateName, bsi));

    bsi = new BlendStateItem();
    ZeroMemory( &bsi->blendDesc, sizeof( D3D11_BLEND_DESC ) );
    bsi->blendDesc.IndependentBlendEnable = FALSE;
    bsi->blendDesc.AlphaToCoverageEnable = FALSE;
    bsi->blendDesc.RenderTarget[0].BlendEnable = TRUE;
    bsi->blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bsi->blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    bsi->blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    bsi->blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
    bsi->blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    bsi->blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bsi->blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;
    g_pd3dDevice->CreateBlendState( &bsi->blendDesc, &bsi->blendState );
    bsi->stateName = wstring(L"TransparentAdd");
	blendStatesMap.insert(std::pair<wstring, BlendStateItem*>(bsi->stateName, bsi));

    bsi = new BlendStateItem();
    ZeroMemory( &bsi->blendDesc, sizeof( D3D11_BLEND_DESC ) );
    bsi->blendDesc.IndependentBlendEnable = FALSE;
    bsi->blendDesc.AlphaToCoverageEnable = TRUE;
    bsi->blendDesc.RenderTarget[0].BlendEnable = TRUE;
    bsi->blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bsi->blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    bsi->blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    bsi->blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
    bsi->blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    bsi->blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bsi->blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;
    g_pd3dDevice->CreateBlendState( &bsi->blendDesc, &bsi->blendState );
    bsi->stateName = wstring(L"AdditiveBlending" );
	blendStatesMap.insert(std::pair<wstring, BlendStateItem*>(bsi->stateName, bsi));

    return;
}

void EffectsManager::createRasterStates()
{
    RasterStateItem * rsi = new RasterStateItem();
    ZeroMemory( &rsi->rasterDesc, sizeof( D3D11_RASTERIZER_DESC ) );
    rsi->rasterDesc.FillMode = D3D11_FILL_SOLID;
    rsi->rasterDesc.CullMode = D3D11_CULL_NONE;
    rsi->rasterDesc.FrontCounterClockwise = FALSE;
    rsi->rasterDesc.DepthBias = 0;
    rsi->rasterDesc.DepthBiasClamp = 0;
    rsi->rasterDesc.SlopeScaledDepthBias = 0;
    rsi->rasterDesc.DepthClipEnable = FALSE;
    rsi->rasterDesc.ScissorEnable = FALSE;
    rsi->rasterDesc.MultisampleEnable = FALSE;
    rsi->rasterDesc.AntialiasedLineEnable = FALSE;
    g_pd3dDevice->CreateRasterizerState( &rsi->rasterDesc, &rsi->rasterState );
    rsi->rasterName = wstring(L"CullNone" );
	rasterStatesMap.insert(std::pair<wstring, RasterStateItem*>(rsi->rasterName, rsi));

    rsi = new RasterStateItem();
    ZeroMemory( &rsi->rasterDesc, sizeof( D3D11_RASTERIZER_DESC ) );
    rsi->rasterDesc.FillMode = D3D11_FILL_SOLID;
    rsi->rasterDesc.CullMode = D3D11_CULL_BACK;
    rsi->rasterDesc.FrontCounterClockwise = FALSE;
    rsi->rasterDesc.DepthBias = 0;
    rsi->rasterDesc.DepthBiasClamp = 0;
    rsi->rasterDesc.SlopeScaledDepthBias = 0;
    rsi->rasterDesc.DepthClipEnable = FALSE;
    rsi->rasterDesc.ScissorEnable = FALSE;
    rsi->rasterDesc.MultisampleEnable = FALSE;
    rsi->rasterDesc.AntialiasedLineEnable = FALSE;
    g_pd3dDevice->CreateRasterizerState( &rsi->rasterDesc, &rsi->rasterState );
    rsi->rasterName = wstring(L"CullBack" );
	rasterStatesMap.insert(std::pair<wstring, RasterStateItem*>(rsi->rasterName, rsi));

    rsi = new RasterStateItem();
    ZeroMemory( &rsi->rasterDesc, sizeof( D3D11_RASTERIZER_DESC ) );
    rsi->rasterDesc.FillMode = D3D11_FILL_SOLID;
    rsi->rasterDesc.CullMode = D3D11_CULL_BACK;
    rsi->rasterDesc.FrontCounterClockwise = FALSE;
    //rsi->rasterDesc.DepthBias = 1000;
    rsi->rasterDesc.DepthBias = 1;
    rsi->rasterDesc.DepthBiasClamp = 0;
    //rsi->rasterDesc.SlopeScaledDepthBias = 8.0f;
    rsi->rasterDesc.SlopeScaledDepthBias = 0.5f;
    rsi->rasterDesc.DepthClipEnable = FALSE;
    rsi->rasterDesc.ScissorEnable = FALSE;
    rsi->rasterDesc.MultisampleEnable = FALSE;
    rsi->rasterDesc.AntialiasedLineEnable = FALSE;
    g_pd3dDevice->CreateRasterizerState( &rsi->rasterDesc, &rsi->rasterState );
    rsi->rasterName = wstring(L"DepthBiasCullBack" );
	rasterStatesMap.insert(std::pair<wstring, RasterStateItem*>(rsi->rasterName, rsi));

    rsi = new RasterStateItem();
    ZeroMemory( &rsi->rasterDesc, sizeof( D3D11_RASTERIZER_DESC ) );
    rsi->rasterDesc.FillMode = D3D11_FILL_SOLID;
    rsi->rasterDesc.CullMode = D3D11_CULL_FRONT;
    rsi->rasterDesc.FrontCounterClockwise = TRUE;
    rsi->rasterDesc.DepthBias = 0;
    rsi->rasterDesc.DepthBiasClamp = 0;
    rsi->rasterDesc.DepthClipEnable = TRUE;
    rsi->rasterDesc.SlopeScaledDepthBias = 0;
    rsi->rasterDesc.ScissorEnable = FALSE;
    rsi->rasterDesc.MultisampleEnable = FALSE;
    rsi->rasterDesc.AntialiasedLineEnable = FALSE;
    g_pd3dDevice->CreateRasterizerState( &rsi->rasterDesc, &rsi->rasterState );
    rsi->rasterName = wstring(L"CullFront" );
	rasterStatesMap.insert(std::pair<wstring, RasterStateItem*>(rsi->rasterName, rsi));

    rsi = new RasterStateItem();
    ZeroMemory( &rsi->rasterDesc, sizeof( D3D11_RASTERIZER_DESC ) );
    rsi->rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
    rsi->rasterDesc.CullMode = D3D11_CULL_NONE;
    rsi->rasterDesc.FrontCounterClockwise = TRUE;
    rsi->rasterDesc.DepthBias = 0;
    rsi->rasterDesc.DepthBiasClamp = 0;
    rsi->rasterDesc.SlopeScaledDepthBias = 0;
    rsi->rasterDesc.ScissorEnable = FALSE;
    rsi->rasterDesc.MultisampleEnable = TRUE;
    rsi->rasterDesc.AntialiasedLineEnable = FALSE;
    g_pd3dDevice->CreateRasterizerState( &rsi->rasterDesc, &rsi->rasterState );
    rsi->rasterName = wstring(L"WireFrame" );
	rasterStatesMap.insert(std::pair<wstring, RasterStateItem*>(rsi->rasterName, rsi));

    return;
}

void EffectsManager::createDepthStencilStates()
{
    DepthStencilStateItem * dssi = new DepthStencilStateItem();
    ZeroMemory( &dssi->depthStencilDesc, sizeof( D3D11_DEPTH_STENCIL_DESC ) );
	dssi->depthStencilDesc.DepthEnable = TRUE;
	dssi->depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssi->depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    dssi->depthStencilDesc.StencilEnable = FALSE;
    dssi->depthStencilDesc.StencilReadMask = 0xff;
    dssi->depthStencilDesc.StencilWriteMask = 0xff;
    dssi->depthStencilDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
    dssi->depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dssi->depthStencilDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
    dssi->depthStencilDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
    dssi->depthStencilDesc.BackFace.StencilFailOp       = D3D11_STENCIL_OP_KEEP;
    dssi->depthStencilDesc.BackFace.StencilDepthFailOp  = D3D11_STENCIL_OP_KEEP;
    dssi->depthStencilDesc.BackFace.StencilPassOp       = D3D11_STENCIL_OP_KEEP;
    dssi->depthStencilDesc.BackFace.StencilFunc         = D3D11_COMPARISON_ALWAYS;
	g_pd3dDevice->CreateDepthStencilState( &dssi->depthStencilDesc, &dssi->depthStencilState );
    dssi->depthStencilName = wstring(L"EnableDepthTestWrite" );
	depthStencilStatesMap.insert(std::pair<wstring, DepthStencilStateItem*>(dssi->depthStencilName, dssi));

    dssi = new DepthStencilStateItem();
    ZeroMemory( &dssi->depthStencilDesc, sizeof( D3D11_DEPTH_STENCIL_DESC ) );
	dssi->depthStencilDesc.DepthEnable = TRUE;
	dssi->depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dssi->depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    dssi->depthStencilDesc.StencilEnable = FALSE;
    dssi->depthStencilDesc.StencilReadMask = 0xff;
    dssi->depthStencilDesc.StencilWriteMask = 0xff;
    dssi->depthStencilDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
    dssi->depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dssi->depthStencilDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
    dssi->depthStencilDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
    dssi->depthStencilDesc.BackFace.StencilFailOp       = D3D11_STENCIL_OP_KEEP;
    dssi->depthStencilDesc.BackFace.StencilDepthFailOp  = D3D11_STENCIL_OP_KEEP;
    dssi->depthStencilDesc.BackFace.StencilPassOp       = D3D11_STENCIL_OP_KEEP;
    dssi->depthStencilDesc.BackFace.StencilFunc         = D3D11_COMPARISON_ALWAYS;
	g_pd3dDevice->CreateDepthStencilState( &dssi->depthStencilDesc, &dssi->depthStencilState );
    dssi->depthStencilName = wstring(L"EnableDepth" );
	depthStencilStatesMap.insert(std::pair<wstring, DepthStencilStateItem*>(dssi->depthStencilName, dssi));

    dssi = new DepthStencilStateItem();
    ZeroMemory( &dssi->depthStencilDesc, sizeof( D3D11_DEPTH_STENCIL_DESC ) );
	dssi->depthStencilDesc.DepthEnable = FALSE;
	dssi->depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dssi->depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    dssi->depthStencilDesc.StencilEnable = FALSE;
    dssi->depthStencilDesc.StencilReadMask = 0xff;
    dssi->depthStencilDesc.StencilWriteMask = 0xff;
    dssi->depthStencilDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
    dssi->depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dssi->depthStencilDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
    dssi->depthStencilDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
    dssi->depthStencilDesc.BackFace.StencilFailOp       = D3D11_STENCIL_OP_KEEP;
    dssi->depthStencilDesc.BackFace.StencilDepthFailOp  = D3D11_STENCIL_OP_KEEP;
    dssi->depthStencilDesc.BackFace.StencilPassOp       = D3D11_STENCIL_OP_KEEP;
    dssi->depthStencilDesc.BackFace.StencilFunc         = D3D11_COMPARISON_ALWAYS;
	g_pd3dDevice->CreateDepthStencilState( &dssi->depthStencilDesc, &dssi->depthStencilState );
    dssi->depthStencilName = wstring(L"NoDepthWrites" );
	depthStencilStatesMap.insert(std::pair<wstring, DepthStencilStateItem*>(dssi->depthStencilName, dssi));

    return;
}

void EffectsManager::createConstantBuffers()
{
    // CB_LOOKUP enum needs to be updated if any additions, reordering, or renaming occurs

    HRESULT hr = S_OK;
    ConstantBufferItem * cbi = new ConstantBufferItem();
    cbi->pBff = NULL;
    ZeroMemory( &cbi->cbDesc, sizeof(D3D11_BUFFER_DESC) );
    cbi->cbDesc.ByteWidth = sizeof( LIGHTEQ_EF_CONSTANT_BUFFER ); 
    cbi->cbDesc.Usage = D3D11_USAGE_DYNAMIC;                      
    cbi->cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;           
    cbi->cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;          
    cbi->cbDesc.MiscFlags = 0;                                    
    hr = g_pd3dDevice->CreateBuffer( &cbi->cbDesc, NULL, &cbi->pBff );
    cbi->constantBufferName = wstring(L"LightEQ" );
	constantBuffersMap.insert(std::pair<wstring, ConstantBufferItem*>(cbi->constantBufferName, cbi));

    cbi = new ConstantBufferItem();
    cbi->pBff = NULL;
    ZeroMemory( &cbi->cbDesc, sizeof(D3D11_BUFFER_DESC) );
    cbi->cbDesc.ByteWidth = sizeof( LIGHTEQ_EF_VS_CONSTANT_BUFFER ); 
    cbi->cbDesc.Usage = D3D11_USAGE_DYNAMIC;                      
    cbi->cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;           
    cbi->cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;          
    cbi->cbDesc.MiscFlags = 0;                                    
    hr = g_pd3dDevice->CreateBuffer( &cbi->cbDesc, NULL, &cbi->pBff );
    cbi->constantBufferName = wstring(L"LIGHTEQ_EF_VS" );
	constantBuffersMap.insert(std::pair<wstring, ConstantBufferItem*>(cbi->constantBufferName, cbi));

    cbi = new ConstantBufferItem();
    cbi->pBff = NULL;
    ZeroMemory( &cbi->cbDesc, sizeof(D3D11_BUFFER_DESC) );
    cbi->cbDesc.ByteWidth = sizeof( LIGHTEQ_EF_PS_CONSTANT_BUFFER ); 
    cbi->cbDesc.Usage = D3D11_USAGE_DYNAMIC;                      
    cbi->cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;           
    cbi->cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;          
    cbi->cbDesc.MiscFlags = 0;                                    
    hr = g_pd3dDevice->CreateBuffer( &cbi->cbDesc, NULL, &cbi->pBff );
    cbi->constantBufferName = wstring(L"LIGHTEQ_EF_PS" );
	constantBuffersMap.insert(std::pair<wstring, ConstantBufferItem*>(cbi->constantBufferName, cbi));

    cbi = new ConstantBufferItem();
    cbi->pBff = NULL;
    ZeroMemory( &cbi->cbDesc, sizeof(D3D11_BUFFER_DESC) );
    cbi->cbDesc.ByteWidth = sizeof( PER_ITEM_CONSTANT_BUFFER_B1 );
    cbi->cbDesc.Usage = D3D11_USAGE_DYNAMIC;                      
    cbi->cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;           
    cbi->cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;          
    cbi->cbDesc.MiscFlags = 0;                                    
    hr = g_pd3dDevice->CreateBuffer( &cbi->cbDesc, NULL, &cbi->pBff );
    cbi->constantBufferName = wstring(L"WORLD_EF_PER_OBJECT" );
	constantBuffersMap.insert(std::pair<wstring, ConstantBufferItem*>(cbi->constantBufferName, cbi));

    cbi = new ConstantBufferItem();
    cbi->pBff = NULL;
    ZeroMemory( &cbi->cbDesc, sizeof(D3D11_BUFFER_DESC) );
    cbi->cbDesc.ByteWidth = sizeof( SHADOW_EF_CONSTANT_BUFFER_B0 );
    cbi->cbDesc.Usage = D3D11_USAGE_DYNAMIC;                      
    cbi->cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;           
    cbi->cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;          
    cbi->cbDesc.MiscFlags = 0;                                    
    hr = g_pd3dDevice->CreateBuffer( &cbi->cbDesc, NULL, &cbi->pBff );
    cbi->constantBufferName = wstring(L"SHADOW_EF_PER_FRAME" );
	constantBuffersMap.insert(std::pair<wstring, ConstantBufferItem*>(cbi->constantBufferName, cbi));

    cbi = new ConstantBufferItem();
    cbi->pBff = NULL;
    ZeroMemory( &cbi->cbDesc, sizeof(D3D11_BUFFER_DESC) );
    cbi->cbDesc.ByteWidth = sizeof( PER_ITEM_CONSTANT_BUFFER_B1 );
    cbi->cbDesc.Usage = D3D11_USAGE_DYNAMIC;                      
    cbi->cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;           
    cbi->cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;          
    cbi->cbDesc.MiscFlags = 0;                                    
    hr = g_pd3dDevice->CreateBuffer( &cbi->cbDesc, NULL, &cbi->pBff );
	cbi->constantBufferName = wstring(L"SHADOW_EF_PER_OBJECT" );
	constantBuffersMap.insert(std::pair<wstring, ConstantBufferItem*>(cbi->constantBufferName, cbi));

    cbi = new ConstantBufferItem();
    ZeroMemory( &cbi->cbDesc, sizeof(D3D11_BUFFER_DESC) );
    cbi->pBff = NULL;
    cbi->constantBufferName = wstring(L"NULL" );
	constantBuffersMap.insert(std::pair<wstring, ConstantBufferItem*>(cbi->constantBufferName, cbi));
	return;
}

effectPacketItem* EffectsManager::createEffect ( wstring edfPath )
{
    effectPacketItem * epi = new effectPacketItem[1];
    bool itemMemberFound = false;
    edfFileItem edfFI;
    VertexShaderItem *      vertexShader;
    PixelShaderItem *       pixelShader;
    BlendStateItem *        blendState;
    RasterStateItem *       rasterState;
    DepthStencilStateItem * depthStencilState;
    ConstantBufferItem *    constantBuffer;
    InputLayoutItem*        inputLayout;

    HRESULT hr = S_OK;
    readEDF( edfPath, edfFI );

    int loop = 0;
    //create vertexShader;
    //get vertexshaderFile
    //check against existing inventory

	auto vsMapItr = vertexShadersMap.find(edfFI.vsEntryPoint);
	if (vsMapItr != vertexShadersMap.end())
	{
		itemMemberFound = true;
		epi->vertexShader = vsMapItr->second;
	}

    //create if unique
    DWORD dwShaderFlags = 0;
    if (!itemMemberFound ) 
    {
        ID3DBlob* pVSBuf = NULL;
#if defined(DEBUG) | defined(_DEBUG)
        dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif
        hr = D3DX11CompileFromFile( edfFI.VSFilePath.c_str(), NULL, NULL, edfFI.cVsEntryPoint.c_str() , "vs_4_0", dwShaderFlags, NULL, NULL, &pVSBuf, NULL,
                                NULL );
        vertexShader = new VertexShaderItem();
        int buffsize = pVSBuf->GetBufferSize();
        g_pd3dDevice->CreateVertexShader( ( DWORD* )pVSBuf->GetBufferPointer(),
                                              pVSBuf->GetBufferSize(), NULL, &(vertexShader->vtxShader) ); 
        vertexShader->entryName = edfFI.vsEntryPoint;
        vertexShader->shaderData = pVSBuf;
        //add to vertexshaderlist
		vertexShadersMap.insert(std::pair<wstring, VertexShaderItem*>(vertexShader->entryName, vertexShader));
        //add to effectPacket.VertexShader;
        epi->vertexShader = vertexShader;
       
    }
    itemMemberFound = false;

     //create pixelShader;
    //get pixelshaderFile
    //check against existing inventory
	auto psMapItr = pixelShadersMap.find(edfFI.psEntryPoint);
	if (psMapItr != pixelShadersMap.end())
	{
		epi->pixelShader = psMapItr->second;
		itemMemberFound = true;
	}

    //create if unique
    if (!itemMemberFound ) 
    {
        ID3DBlob* pPSBuf = NULL;
        dwShaderFlags = 0;
#if defined(DEBUG) | defined(_DEBUG)
        dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif
        hr = D3DX11CompileFromFile( edfFI.PSFilePath.c_str(), NULL, NULL, edfFI.cPsEntryPoint.c_str(), "ps_4_0", dwShaderFlags, NULL, NULL, &pPSBuf, NULL,
                                NULL );
        pixelShader = new PixelShaderItem();
        g_pd3dDevice->CreatePixelShader( ( DWORD* )pPSBuf->GetBufferPointer(),
                                              pPSBuf->GetBufferSize(), NULL, &(pixelShader->pxlShader) ); 
        pixelShader->entryName = edfFI.psEntryPoint;

        pixelShader->shaderData = pPSBuf;
        //add to pixelshaderlist
		pixelShadersMap.insert(std::pair<wstring, PixelShaderItem*>(pixelShader->entryName, pixelShader));

        //add to effectPacket.PixelShader;
        epi->pixelShader = pixelShader;
       
    }
    itemMemberFound = false;

    //find blendState; 
	auto bsMapItr = blendStatesMap.find(edfFI.blendState);
	if (bsMapItr != blendStatesMap.end())
	{
		epi->blendState = bsMapItr->second;
		itemMemberFound = true;
	}

    if ( !itemMemberFound )
    {
 		MessageBox( NULL,
			L"Error finding valid BlendState - bailing out!", L"Error", MB_OK );
        return NULL;
    }
    itemMemberFound = false;
    //find rasterState;
	auto rsMapItr = rasterStatesMap.find(edfFI.rasterState);
	if (rsMapItr != rasterStatesMap.end())
	{
		epi->rasterState = rsMapItr->second;
		itemMemberFound = true;
	}

    if ( !itemMemberFound )
    {
 		MessageBox( NULL,
			L"Error finding valid RasterState - bailing out!", L"Error", MB_OK );
        return NULL;
    }
    itemMemberFound = false;

    //find depthStencelState;
	auto dsMapItr = depthStencilStatesMap.find(edfFI.depthStencilState);
	if (dsMapItr != depthStencilStatesMap.end())
	{
		epi->depthStencilState = dsMapItr->second;
		itemMemberFound = true;
	}
    if ( !itemMemberFound )
    {
 		MessageBox( NULL,
			L"Error finding valid DepthStencilState - bailing out!", L"Error", MB_OK );
        return NULL;
    }
    itemMemberFound = false;

    //find vsConstantBuffer;
	auto vscsBufferMapItr = constantBuffersMap.find(edfFI.vsConstantBuffer);
	if (vscsBufferMapItr != constantBuffersMap.end())
	{
		epi->vsConstantBuffer = vscsBufferMapItr->second;
		itemMemberFound = true;
	}
    if ( !itemMemberFound )
    {
 		MessageBox( NULL,
			L"Error finding valid vsConstantBuffer - bailing out!", L"Error", MB_OK );
        return NULL;
    }
    itemMemberFound = false;

    //find psConstantBuffer;
	auto pscsBufferMapItr = constantBuffersMap.find(edfFI.psConstantBuffer);
	if (pscsBufferMapItr != constantBuffersMap.end())
	{
		epi->psConstantBuffer = pscsBufferMapItr->second;
		itemMemberFound = true;
	}
	if ( !itemMemberFound )
    {
 		MessageBox( NULL,
			L"Error finding valid vsConstantBuffer - bailing out!", L"Error", MB_OK );
        return NULL;
    }
    itemMemberFound = false;

    //find samplerState;
	auto ssMapItr = samplerStatesMap.find(edfFI.samplerState);
	if (ssMapItr != samplerStatesMap.end())
	{
		epi->samplerState = ssMapItr->second;
		itemMemberFound = true;
	}
	if ( !itemMemberFound )
    {
 		MessageBox( NULL,
			L"Error finding valid SamplerState - bailing out!", L"Error", MB_OK );
        return NULL;
    }
    itemMemberFound = false;

    epi->effectMask = edfFI.EffectMask;
    return epi;

}

InputLayoutItem* EffectsManager::InitLayouts( ILOType iloType, effectPacketItem *epi)
{
 
    ID3D11InputLayout*      VertexLayout = NULL;
    wstring iloName;
    HRESULT hr = S_OK;
    switch (iloType){
            case ILOType::P:
            {
                iloName = wstring(L"P" );
				auto lOItr = inputLayoutsMap.find(iloName);
				if (lOItr != inputLayoutsMap.end())
					return lOItr->second;
				//
	            // Position vertex
	            //
 	            D3D11_INPUT_ELEMENT_DESC posVertexDesc[] =
	            {
		            {"POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0}
	            };

                hr = g_pd3dDevice->CreateInputLayout(posVertexDesc, sizeof( posVertexDesc ) / sizeof( D3D11_INPUT_ELEMENT_DESC ), epi->vertexShader->shaderData->GetBufferPointer(),
	                                                 epi->vertexShader->shaderData->GetBufferSize(), &VertexLayout);
                break;
            }
        case PNT: // Position-normal-texture vertex
            {
				iloName = wstring(L"PNT" );
				auto lOItr = inputLayoutsMap.find(iloName);
				if (lOItr != inputLayoutsMap.end())
					return lOItr->second;
	            D3D11_INPUT_ELEMENT_DESC posNormalTexVertexDesc[] =
	            {
		            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		            {"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
	            };
                hr = g_pd3dDevice->CreateInputLayout(posNormalTexVertexDesc, sizeof( posNormalTexVertexDesc ) / sizeof( D3D11_INPUT_ELEMENT_DESC ), epi->vertexShader->shaderData->GetBufferPointer(),
	                                                 epi->vertexShader->shaderData->GetBufferSize(), &VertexLayout);            
                break;
            }
        case PTNT: // Position-tangent-normal-texture vertex
            {
                iloName = wstring(L"PTNT" );
				auto lOItr = inputLayoutsMap.find(iloName);
				if (lOItr != inputLayoutsMap.end())
					return lOItr->second;
	            D3D11_INPUT_ELEMENT_DESC PosTanNormVertexDesc[] =
	            {
		            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		            {"TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		            {"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0},
	            };

                hr = g_pd3dDevice->CreateInputLayout(PosTanNormVertexDesc, sizeof( PosTanNormVertexDesc ) / sizeof( D3D11_INPUT_ELEMENT_DESC ), epi->vertexShader->shaderData->GetBufferPointer(),
	                                                 epi->vertexShader->shaderData->GetBufferSize(), &VertexLayout);
                break;
            }
        case PVSAT:// Particle vertex
            {
                iloName = wstring(L"PVSAT" );
				auto lOItr = inputLayoutsMap.find(iloName);
				if (lOItr != inputLayoutsMap.end())
					return lOItr->second;
	            D3D11_INPUT_ELEMENT_DESC particleDesc[] =
	            {
		            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		            {"VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		            {"SIZE",     0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		            {"AGE",      0, DXGI_FORMAT_R32_FLOAT,       0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		            {"TYPE",     0, DXGI_FORMAT_R32_UINT,        0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0},
	            };

                hr = g_pd3dDevice->CreateInputLayout(particleDesc, sizeof( particleDesc ) / sizeof( D3D11_INPUT_ELEMENT_DESC ), epi->vertexShader->shaderData->GetBufferPointer(),
	                                                 epi->vertexShader->shaderData->GetBufferSize(), &VertexLayout);
                break;
            }
        case PC:// Square vertex
            {
                iloName = wstring(L"PC" );
				auto lOItr = inputLayoutsMap.find(iloName);
				if (lOItr != inputLayoutsMap.end())
					return lOItr->second;
				D3D11_INPUT_ELEMENT_DESC squareDesc[] =
	            {
                    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
                    {"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	            };

                hr = g_pd3dDevice->CreateInputLayout(squareDesc, sizeof( squareDesc ) / sizeof( D3D11_INPUT_ELEMENT_DESC ), epi->vertexShader->shaderData->GetBufferPointer(),
	                                                 epi->vertexShader->shaderData->GetBufferSize(), &VertexLayout);
                break;
            }
        case PT:// Square vertex
            {
                iloName = wstring(L"PT" );
				auto lOItr = inputLayoutsMap.find(iloName);
				if (lOItr != inputLayoutsMap.end())
					return lOItr->second;
	            D3D11_INPUT_ELEMENT_DESC PTDesc[] =
	            {
                    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	            };

                hr = g_pd3dDevice->CreateInputLayout(PTDesc, sizeof( PTDesc ) / sizeof( D3D11_INPUT_ELEMENT_DESC ), epi->vertexShader->shaderData->GetBufferPointer(),
	                                                 epi->vertexShader->shaderData->GetBufferSize(), &VertexLayout);
                break;
            }
    }
    InputLayoutItem * ilo = new InputLayoutItem[1];
    ilo->IPLO = VertexLayout;
    ilo->inputLayoutName = iloName;
	inputLayoutsMap.insert(std::pair<wstring, InputLayoutItem*>(ilo->inputLayoutName, ilo));
    return ilo;
}
