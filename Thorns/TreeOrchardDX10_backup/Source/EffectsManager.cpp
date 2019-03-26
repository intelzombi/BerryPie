#include "EffectsManager.h"
//#include "Global.h"


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
    for( int loop = 0; loop < (int)effectsList.size() ; loop++)
    {
        if( effectsList[loop]->effect )
            effectsList[loop]->effect->Release();

        delete effectsList[loop];
    }

    for( int loop = 0; loop < (int)vertexShadersList.size() ; loop++)
    {
        if( vertexShadersList[loop]->shaderData )
            vertexShadersList[loop]->shaderData->Release();
        if( vertexShadersList[loop]->vtxShader )
            vertexShadersList[loop]->vtxShader->Release();

        delete vertexShadersList[loop];
    }

    for( int loop = 0; loop < (int)pixelShadersList.size() ; loop++)
    {
        if( pixelShadersList[loop]->shaderData )
            pixelShadersList[loop]->shaderData->Release();
        if( pixelShadersList[loop]->pxlShader)
            pixelShadersList[loop]->pxlShader->Release();

        delete pixelShadersList[loop];
    }

    for( int loop = 0; loop < (int)blendStatesList.size() ; loop++)
    {
        if( blendStatesList[loop]->blendState )
            blendStatesList[loop]->blendState->Release();

        delete blendStatesList[loop];
    }

    for( int loop = 0; loop < (int)rasterStatesList.size() ; loop++)
    {
        if( rasterStatesList[loop]->rasterState )
            rasterStatesList[loop]->rasterState->Release();

        delete rasterStatesList[loop];
    }

    for( int loop = 0; loop < (int)depthStencilStatesList.size() ; loop++)
    {
        if( depthStencilStatesList[loop]->DepthStencilState )
            depthStencilStatesList[loop]->DepthStencilState->Release();

        delete depthStencilStatesList[loop];
    }

    for( int loop = 0; loop < (int)constantBuffersList.size() ; loop++)
    {
        if( constantBuffersList[loop]->pBff )
            constantBuffersList[loop]->pBff->Release();

        delete constantBuffersList[loop];
    }

    for( int loop = 0; loop < (int)samplerStatesList.size() ; loop++)
    {
        if( samplerStatesList[loop]->samplerState )
            samplerStatesList[loop]->samplerState->Release();

        delete samplerStatesList[loop];
    }


    for( int loop = 0; loop < (int)inputLayoutsList.size() ; loop++)
    {
        if( inputLayoutsList[loop]->IPLO )
            inputLayoutsList[loop]->IPLO->Release();

        delete inputLayoutsList[loop];
    }

}

EffectsManager::~EffectsManager()
{
    effectsList.clear();
}

bool  EffectsManager::readEDF( TCHAR *Path , edfFileItem &edfFI )
{
    FILE *effectDefFile = NULL;
    TCHAR fileItem[256];
    effectDefFile = _wfopen( Path, L"r");
 	if ( !effectDefFile )
	{
 		MessageBox( NULL,
			L"Error opening model definition file - bailing out!", L"Error", MB_OK );
        return false;
    }
    wchar_t* bs = NULL;
    bs = wcsrchr(Path, L'\\');

    if ( wcslen(bs) > 255 ) 
    {
 		MessageBox( NULL,
			L"Your effect definition file is greater than 255 characters - bailing out!", L"Error", MB_OK );
        return false;

    }
    
    //truncate "path\\filename" to just "path\\"
    wcscpy( bs +1, L"\0"); 

    wcscpy(edfFI.path, Path);
   
    fwscanf( effectDefFile, L"VertexShader		    %s\n", &fileItem );
    wcscpy( bs+1, fileItem );
    wcscpy( edfFI.VSFile, fileItem );
    wcscpy( edfFI.VSFilePath, Path );
    fwscanf( effectDefFile, L"VSEntryPoint		    %s\n", &edfFI.vsEntryPoint );
    char item[256];
    WideCharToMultiByte(CP_ACP,0, edfFI.vsEntryPoint,-1,item,256,0,0);
    strcpy(edfFI.cVsEntryPoint, item );
    fwscanf( effectDefFile, L"VSConstantBuffer	    %s\n", &edfFI.vsConstantBuffer );
    fwscanf( effectDefFile, L"PixelShader		    %s\n", &fileItem );
    wcscpy( bs+1, fileItem );
    wcscpy( edfFI.PSFile, fileItem );
    wcscpy( edfFI.PSFilePath, Path );
    fwscanf( effectDefFile, L"PSEntryPoint		    %s\n", &edfFI.psEntryPoint );
    WideCharToMultiByte(CP_ACP,0, edfFI.psEntryPoint,-1,item,256,0,0);
    strcpy(edfFI.cPsEntryPoint, item );
    fwscanf( effectDefFile, L"PSConstantBuffer		%s\n", &edfFI.psConstantBuffer );
    fwscanf( effectDefFile, L"BlendState		    %s\n", &edfFI.blendState );
    fwscanf( effectDefFile, L"RasterizerState	    %s\n", &edfFI.rasterState );
    fwscanf( effectDefFile, L"DepthStencilState	    %s\n", &edfFI.depthStencilState );
    fwscanf( effectDefFile, L"SamplerState	        %s\n", &edfFI.samplerState );
    fwscanf( effectDefFile, L"InputLayout		    %s\n", &edfFI.ILO );
   
}

void EffectsManager::createSamplerStates()
{
    SamplerStateItem * ssi = new SamplerStateItem();
    ssi->samplerState = NULL;
    ZeroMemory( &ssi->samplerDesc, sizeof( D3D10_SAMPLER_DESC ) );
    ssi->samplerDesc.Filter = D3D10_FILTER_MIN_MAG_MIP_LINEAR;
    ssi->samplerDesc.AddressU = D3D10_TEXTURE_ADDRESS_WRAP;
    ssi->samplerDesc.AddressV = D3D10_TEXTURE_ADDRESS_WRAP;
    ssi->samplerDesc.AddressW = D3D10_TEXTURE_ADDRESS_WRAP;
    ssi->samplerDesc.MipLODBias = 0.0f;
    ssi->samplerDesc.MaxAnisotropy = 16;
    ssi->samplerDesc.ComparisonFunc = D3D10_COMPARISON_NEVER;
    //ssi->blendDesc.BorderColor = {0.0f, 0.0f, 0.0f, 0.0f};
    ssi->samplerDesc.MinLOD = 0.0f;
    ssi->samplerDesc.MaxLOD = FLT_MAX;
    g_pd3dDevice->CreateSamplerState( &ssi->samplerDesc, &ssi->samplerState );
    wcscpy( ssi->samplerName, L"simpleSampler" );
    samplerStatesList.push_back( ssi );
    return;
}

void EffectsManager::createBlendStates()
{
    BlendStateItem * bsi = new BlendStateItem();
    bsi->blendState = NULL;
    ZeroMemory( &bsi->blendDesc, sizeof( D3D10_BLEND_DESC ) );
    bsi->blendDesc.BlendEnable[0] = FALSE;
    bsi->blendDesc.AlphaToCoverageEnable = FALSE;
    bsi->blendDesc.SrcBlend = D3D10_BLEND_ONE;
    bsi->blendDesc.DestBlend = D3D10_BLEND_ZERO;
    bsi->blendDesc.BlendOp = D3D10_BLEND_OP_ADD;
    bsi->blendDesc.SrcBlendAlpha = D3D10_BLEND_ONE;
    bsi->blendDesc.DestBlendAlpha = D3D10_BLEND_ZERO;
    bsi->blendDesc.BlendOpAlpha = D3D10_BLEND_OP_ADD;
    bsi->blendDesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;
    g_pd3dDevice->CreateBlendState( &bsi->blendDesc, &bsi->blendState );
    wcscpy( bsi->stateName, L"NoBlending" );
    blendStatesList.push_back( bsi );

    bsi = new BlendStateItem();
    ZeroMemory( &bsi->blendDesc, sizeof( D3D10_BLEND_DESC ) );
    bsi->blendDesc.BlendEnable[0] = TRUE;
    bsi->blendDesc.AlphaToCoverageEnable = FALSE;
    bsi->blendDesc.SrcBlend = D3D10_BLEND_SRC_ALPHA;
    bsi->blendDesc.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
    bsi->blendDesc.BlendOp = D3D10_BLEND_OP_ADD;
    bsi->blendDesc.SrcBlendAlpha = D3D10_BLEND_ZERO;
    bsi->blendDesc.DestBlendAlpha = D3D10_BLEND_ZERO;
    bsi->blendDesc.BlendOpAlpha = D3D10_BLEND_OP_ADD;
    bsi->blendDesc.RenderTargetWriteMask[0] = 0x0F;
    g_pd3dDevice->CreateBlendState( &bsi->blendDesc, &bsi->blendState );
    wcscpy( bsi->stateName, L"TransparentAdd" );
    blendStatesList.push_back( bsi );

    bsi = new BlendStateItem();
    ZeroMemory( &bsi->blendDesc, sizeof( D3D10_BLEND_DESC ) );
    bsi->blendDesc.BlendEnable[0] = TRUE;
    bsi->blendDesc.AlphaToCoverageEnable = TRUE;
    bsi->blendDesc.SrcBlend = D3D10_BLEND_SRC_ALPHA;
    bsi->blendDesc.DestBlend = D3D10_BLEND_ONE;
    bsi->blendDesc.BlendOp = D3D10_BLEND_OP_ADD;
    bsi->blendDesc.SrcBlendAlpha = D3D10_BLEND_ZERO;
    bsi->blendDesc.DestBlendAlpha = D3D10_BLEND_ZERO;
    bsi->blendDesc.BlendOpAlpha = D3D10_BLEND_OP_ADD;
    bsi->blendDesc.RenderTargetWriteMask[0] = 0x0F;
    g_pd3dDevice->CreateBlendState( &bsi->blendDesc, &bsi->blendState );
    wcscpy( bsi->stateName, L"AdditiveBlending" );
    blendStatesList.push_back( bsi );

    return;
}

void EffectsManager::createRasterStates()
{
    RasterStateItem * rsi = new RasterStateItem();
    ZeroMemory( &rsi->rasterDesc, sizeof( D3D10_RASTERIZER_DESC ) );
    rsi->rasterDesc.FillMode = D3D10_FILL_SOLID;
    rsi->rasterDesc.CullMode = D3D10_CULL_NONE;
    rsi->rasterDesc.FrontCounterClockwise = FALSE;
    rsi->rasterDesc.DepthBias = 0;
    rsi->rasterDesc.DepthBiasClamp = 0;
    rsi->rasterDesc.DepthClipEnable = TRUE;
    rsi->rasterDesc.SlopeScaledDepthBias = 0;
    rsi->rasterDesc.ScissorEnable = FALSE;
    rsi->rasterDesc.MultisampleEnable = FALSE;
    rsi->rasterDesc.AntialiasedLineEnable = FALSE;
    g_pd3dDevice->CreateRasterizerState( &rsi->rasterDesc, &rsi->rasterState );
    wcscpy( rsi->rasterName, L"CullNone" );
    rasterStatesList.push_back( rsi );

    rsi = new RasterStateItem();
    ZeroMemory( &rsi->rasterDesc, sizeof( D3D10_RASTERIZER_DESC ) );
    rsi->rasterDesc.FillMode = D3D10_FILL_SOLID;
    rsi->rasterDesc.CullMode = D3D10_CULL_BACK;
    rsi->rasterDesc.FrontCounterClockwise = FALSE;
    rsi->rasterDesc.DepthBias = 0;
    rsi->rasterDesc.DepthBiasClamp = 0;
    rsi->rasterDesc.DepthClipEnable = TRUE;
    rsi->rasterDesc.SlopeScaledDepthBias = 0;
    rsi->rasterDesc.ScissorEnable = FALSE;
    rsi->rasterDesc.MultisampleEnable = TRUE;
    rsi->rasterDesc.AntialiasedLineEnable = FALSE;
    g_pd3dDevice->CreateRasterizerState( &rsi->rasterDesc, &rsi->rasterState );
    wcscpy( rsi->rasterName, L"CullBack" );
    rasterStatesList.push_back( rsi );

    rsi = new RasterStateItem();
    ZeroMemory( &rsi->rasterDesc, sizeof( D3D10_RASTERIZER_DESC ) );
    rsi->rasterDesc.FillMode = D3D10_FILL_SOLID;
    rsi->rasterDesc.CullMode = D3D10_CULL_FRONT;
    rsi->rasterDesc.FrontCounterClockwise = TRUE;
    rsi->rasterDesc.DepthBias = 0;
    rsi->rasterDesc.DepthClipEnable = TRUE;
    rsi->rasterDesc.DepthBiasClamp = 0;
    rsi->rasterDesc.SlopeScaledDepthBias = 0;
    rsi->rasterDesc.ScissorEnable = FALSE;
    rsi->rasterDesc.MultisampleEnable = TRUE;
    rsi->rasterDesc.AntialiasedLineEnable = FALSE;
    g_pd3dDevice->CreateRasterizerState( &rsi->rasterDesc, &rsi->rasterState );
    wcscpy( rsi->rasterName, L"CullFront" );
    rasterStatesList.push_back( rsi );

    rsi = new RasterStateItem();
//    ZeroMemory( &rsi->rasterDesc, sizeof( D3D10_RASTERIZER_DESC ) );
    rsi->rasterDesc.FillMode = D3D10_FILL_WIREFRAME;
    rsi->rasterDesc.CullMode = D3D10_CULL_NONE;
    rsi->rasterDesc.FrontCounterClockwise = TRUE;
    rsi->rasterDesc.DepthBias = 0;
    rsi->rasterDesc.DepthBiasClamp = 0;
    rsi->rasterDesc.SlopeScaledDepthBias = 0;
    rsi->rasterDesc.ScissorEnable = FALSE;
    rsi->rasterDesc.MultisampleEnable = TRUE;
    rsi->rasterDesc.AntialiasedLineEnable = FALSE;
    g_pd3dDevice->CreateRasterizerState( &rsi->rasterDesc, &rsi->rasterState );
    wcscpy( rsi->rasterName, L"WireFrame" );
    rasterStatesList.push_back( rsi );

    return;
}

void EffectsManager::createDepthStencilStates()
{
    DepthStencilStateItem * dssi = new DepthStencilStateItem();
    ZeroMemory( &dssi->DepthStencilDesc, sizeof( D3D10_DEPTH_STENCIL_DESC ) );
	dssi->DepthStencilDesc.DepthEnable = TRUE;
	dssi->DepthStencilDesc.DepthFunc = D3D10_COMPARISON_LESS;
	dssi->DepthStencilDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
    dssi->DepthStencilDesc.StencilEnable = FALSE;
    dssi->DepthStencilDesc.StencilReadMask = 0xff;
    dssi->DepthStencilDesc.StencilWriteMask = 0xff;
    dssi->DepthStencilDesc.FrontFace.StencilFailOp      = D3D10_STENCIL_OP_KEEP;
    dssi->DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D10_STENCIL_OP_KEEP;
    dssi->DepthStencilDesc.FrontFace.StencilPassOp      = D3D10_STENCIL_OP_KEEP;
    dssi->DepthStencilDesc.FrontFace.StencilFunc        = D3D10_COMPARISON_ALWAYS;
    dssi->DepthStencilDesc.BackFace.StencilFailOp       = D3D10_STENCIL_OP_KEEP;
    dssi->DepthStencilDesc.BackFace.StencilDepthFailOp  = D3D10_STENCIL_OP_KEEP;
    dssi->DepthStencilDesc.BackFace.StencilPassOp       = D3D10_STENCIL_OP_KEEP;
    dssi->DepthStencilDesc.BackFace.StencilFunc         = D3D10_COMPARISON_ALWAYS;
	g_pd3dDevice->CreateDepthStencilState( &dssi->DepthStencilDesc, &dssi->DepthStencilState );
    wcscpy( dssi->depthStencilName, L"EnableDepthTestWrite" );
    depthStencilStatesList.push_back( dssi );

    dssi = new DepthStencilStateItem();
    ZeroMemory( &dssi->DepthStencilDesc, sizeof( D3D10_DEPTH_STENCIL_DESC ) );
	dssi->DepthStencilDesc.DepthEnable = FALSE;
	dssi->DepthStencilDesc.DepthFunc = D3D10_COMPARISON_EQUAL;
	dssi->DepthStencilDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ZERO;
	g_pd3dDevice->CreateDepthStencilState( &dssi->DepthStencilDesc, &dssi->DepthStencilState );
    wcscpy( dssi->depthStencilName, L"NoDepthWrites" );
    depthStencilStatesList.push_back( dssi );

    dssi = new DepthStencilStateItem();
    ZeroMemory( &dssi->DepthStencilDesc, sizeof( D3D10_DEPTH_STENCIL_DESC ) );
	dssi->DepthStencilDesc.DepthEnable = TRUE;
	dssi->DepthStencilDesc.DepthFunc = D3D10_COMPARISON_LESS;
	dssi->DepthStencilDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ZERO;
    dssi->DepthStencilDesc.StencilEnable = FALSE;
    dssi->DepthStencilDesc.StencilReadMask = 0xff;
    dssi->DepthStencilDesc.StencilWriteMask = 0xff;
    dssi->DepthStencilDesc.FrontFace.StencilFailOp      = D3D10_STENCIL_OP_KEEP;
    dssi->DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D10_STENCIL_OP_KEEP;
    dssi->DepthStencilDesc.FrontFace.StencilPassOp      = D3D10_STENCIL_OP_KEEP;
    dssi->DepthStencilDesc.FrontFace.StencilFunc        = D3D10_COMPARISON_ALWAYS;
    dssi->DepthStencilDesc.BackFace.StencilFailOp       = D3D10_STENCIL_OP_KEEP;
    dssi->DepthStencilDesc.BackFace.StencilDepthFailOp  = D3D10_STENCIL_OP_KEEP;
    dssi->DepthStencilDesc.BackFace.StencilPassOp       = D3D10_STENCIL_OP_KEEP;
    dssi->DepthStencilDesc.BackFace.StencilFunc         = D3D10_COMPARISON_ALWAYS;
	g_pd3dDevice->CreateDepthStencilState( &dssi->DepthStencilDesc, &dssi->DepthStencilState );
    wcscpy( dssi->depthStencilName, L"EnableDepth" );
    depthStencilStatesList.push_back( dssi );


    return;
}

void EffectsManager::createConstantBuffers()
{

    HRESULT hr = S_OK;
    ConstantBufferItem * cbi = new ConstantBufferItem();
    cbi->pBff = NULL;
    //D3D10_BUFFER_DESC cbDesc;
    ZeroMemory( &cbi->cbDesc, sizeof(D3D10_BUFFER_DESC) );
    cbi->cbDesc.ByteWidth = sizeof( LIGHTEQ_EF_CONSTANT_BUFFER ); //
    cbi->cbDesc.Usage = D3D10_USAGE_DYNAMIC;                      //
    cbi->cbDesc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;           //
    cbi->cbDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;          //
    cbi->cbDesc.MiscFlags = 0;                                    //
    hr = g_pd3dDevice->CreateBuffer( &cbi->cbDesc, NULL, &cbi->pBff );
    wcscpy( cbi->constantBufferName, L"LightEQ" );
    constantBuffersList.push_back( cbi );

    cbi = new ConstantBufferItem();
    cbi->pBff = NULL;
    //D3D10_BUFFER_DESC cbDesc;
    ZeroMemory( &cbi->cbDesc, sizeof(D3D10_BUFFER_DESC) );
    cbi->cbDesc.ByteWidth = sizeof( LIGHTEQ_EF_VS_CONSTANT_BUFFER ); //
    cbi->cbDesc.Usage = D3D10_USAGE_DYNAMIC;                      //
    cbi->cbDesc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;           //
    cbi->cbDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;          //
    cbi->cbDesc.MiscFlags = 0;                                    //
    hr = g_pd3dDevice->CreateBuffer( &cbi->cbDesc, NULL, &cbi->pBff );
    wcscpy( cbi->constantBufferName, L"LIGHTEQ_EF_VS" );
    constantBuffersList.push_back( cbi );

    cbi = new ConstantBufferItem();
    cbi->pBff = NULL;
    //D3D10_BUFFER_DESC cbDesc;
    ZeroMemory( &cbi->cbDesc, sizeof(D3D10_BUFFER_DESC) );
    cbi->cbDesc.ByteWidth = sizeof( LIGHTEQ_EF_PS_CONSTANT_BUFFER ); //
    cbi->cbDesc.Usage = D3D10_USAGE_DYNAMIC;                      //
    cbi->cbDesc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;           //
    cbi->cbDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;          //
    cbi->cbDesc.MiscFlags = 0;                                    //
    hr = g_pd3dDevice->CreateBuffer( &cbi->cbDesc, NULL, &cbi->pBff );
    wcscpy( cbi->constantBufferName, L"LIGHTEQ_EF_PS" );
    constantBuffersList.push_back( cbi );
    return;
}

effectPacketItem* EffectsManager::createEffect ( TCHAR *edfPath, int a )
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
    for( loop = 0; loop < (int)vertexShadersList.size(); loop++)
    {
        if ( wcscmp( edfFI.vsEntryPoint, vertexShadersList[loop]->entryName ) == 0 ) 
        {
            epi->vertexShader = vertexShadersList[loop];
            itemMemberFound = true;
            continue;
        }
    }

    //create if unique
    DWORD dwShaderFlags = 0;
    if (!itemMemberFound ) 
    {
        ID3D10Blob* pVSBuf = NULL;
#if defined(DEBUG) | defined(_DEBUG)
        dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif
        hr = D3DX10CompileFromFile( edfFI.VSFilePath, NULL, NULL, edfFI.cVsEntryPoint , "vs_4_0", dwShaderFlags, NULL, NULL, &pVSBuf, NULL,
                                NULL );
        vertexShader = new VertexShaderItem();
        int buffsize = pVSBuf->GetBufferSize();
        g_pd3dDevice->CreateVertexShader( ( DWORD* )pVSBuf->GetBufferPointer(),
                                              pVSBuf->GetBufferSize(), &(vertexShader->vtxShader) ); 
        wcscpy(vertexShader->entryName , edfFI.vsEntryPoint);
        vertexShader->shaderData = pVSBuf;
        //add to vertexshaderlist
        vertexShadersList.push_back(vertexShader);

        //add to effectPacket.VertexShader;
        epi->vertexShader = vertexShader;
       
    }
    itemMemberFound = false;

     //create pixelShader;
    //get pixelshaderFile
    //check against existing inventory
    for( loop = 0; loop < (int)pixelShadersList.size(); loop++)
    {
        if ( wcscmp( edfFI.psEntryPoint, pixelShadersList[loop]->entryName ) == 0 ) 
        {
            epi->pixelShader = pixelShadersList[loop];
            itemMemberFound = true;
            continue;
        }
    }
     
    //create if unique
    if (!itemMemberFound ) 
    {
        ID3D10Blob* pPSBuf = NULL;
        dwShaderFlags = 0;
#if defined(DEBUG) | defined(_DEBUG)
        dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif
        D3DX10CompileFromFile( edfFI.PSFilePath, NULL, NULL, edfFI.cPsEntryPoint , "ps_4_0", dwShaderFlags, NULL, NULL, &pPSBuf, NULL,
                                NULL );
        pixelShader = new PixelShaderItem();
        g_pd3dDevice->CreatePixelShader( ( DWORD* )pPSBuf->GetBufferPointer(),
                                              pPSBuf->GetBufferSize(), &(pixelShader->pxlShader) ); 
        wcscpy(pixelShader->entryName , edfFI.psEntryPoint);

        pixelShader->shaderData = pPSBuf;
        //add to pixelshaderlist
        pixelShadersList.push_back(pixelShader);

        //add to effectPacket.PixelShader;
        epi->pixelShader = pixelShader;
       
    }
    itemMemberFound = false;

    //find blendState; 
    for( loop = 0; loop < (int)blendStatesList.size(); loop++)
    {
        if ( wcscmp( edfFI.blendState, blendStatesList[loop]->stateName ) == 0 ) 
        {
            epi->blendState = blendStatesList[loop];
            itemMemberFound = true;
            continue;
        }
    }
    if ( !itemMemberFound )
    {
 		MessageBox( NULL,
			L"Error finding valid BlendState - bailing out!", L"Error", MB_OK );
        return NULL;
    }
    itemMemberFound = false;
    //find rasterState;
    for( loop = 0; loop < (int)rasterStatesList.size(); loop++)
    {
        if ( wcscmp( edfFI.rasterState, rasterStatesList[loop]->rasterName ) == 0 ) 
        {
            epi->rasterState = rasterStatesList[loop];
            itemMemberFound = true;
            continue;
        }
    }
    if ( !itemMemberFound )
    {
 		MessageBox( NULL,
			L"Error finding valid RasterState - bailing out!", L"Error", MB_OK );
        return NULL;
    }
    itemMemberFound = false;

    //find depthStencelState;
    for( loop = 0; loop < (int)depthStencilStatesList.size(); loop++)
    {
        if ( wcscmp( edfFI.depthStencilState, depthStencilStatesList[loop]->depthStencilName ) == 0 ) 
        {
            epi->depthStencilState = depthStencilStatesList[loop];
            itemMemberFound = true;
            continue;
        }
    }
    if ( !itemMemberFound )
    {
 		MessageBox( NULL,
			L"Error finding valid DepthStencilState - bailing out!", L"Error", MB_OK );
        return NULL;
    }
    itemMemberFound = false;

    //find vsConstantBuffer;
    for( loop = 0; loop < (int)constantBuffersList.size(); loop++)
    {
        if ( wcscmp( edfFI.vsConstantBuffer, constantBuffersList[loop]->constantBufferName ) == 0 ) 
        {
            epi->vsConstantBuffer = constantBuffersList[loop];
            itemMemberFound = true;
            continue;
        }
    }
    if ( !itemMemberFound )
    {
 		MessageBox( NULL,
			L"Error finding valid vsConstantBuffer - bailing out!", L"Error", MB_OK );
        return NULL;
    }
    itemMemberFound = false;

    //find psConstantBuffer;
    for( loop = 0; loop < (int)constantBuffersList.size(); loop++)
    {
        if ( wcscmp( edfFI.psConstantBuffer, constantBuffersList[loop]->constantBufferName ) == 0 ) 
        {
            epi->psConstantBuffer = constantBuffersList[loop];
            itemMemberFound = true;
            continue;
        }
    }
    if ( !itemMemberFound )
    {
 		MessageBox( NULL,
			L"Error finding valid vsConstantBuffer - bailing out!", L"Error", MB_OK );
        return NULL;
    }
    itemMemberFound = false;

    //find samplerState;
    for( loop = 0; loop < (int)samplerStatesList.size(); loop++)
    {
        if ( wcscmp( edfFI.samplerState, samplerStatesList[loop]->samplerName ) == 0 ) 
        {
            epi->samplerState = samplerStatesList[loop];
            itemMemberFound = true;
            continue;
        }
    }
    if ( !itemMemberFound )
    {
 		MessageBox( NULL,
			L"Error finding valid SamplerState - bailing out!", L"Error", MB_OK );
        return NULL;
    }
    itemMemberFound = false;

    return epi;

}

ID3D10Effect* EffectsManager::createEffect (TCHAR *path )
{
    HRESULT hr = S_OK;

    wchar_t* bs = NULL;
    bs = wcsrchr(path, L'\\');

    if ( wcslen(bs) > 255 ) 
    {
 		MessageBox( NULL,
			L"Your shadername is greater than 255 characters - bailing out!", L"Error", MB_OK );
        return NULL;

    }

    for( int loop = 0; loop < (int)effectsList.size(); loop++)
    {
        if ( wcscmp( bs+1, effectsList[loop]->fxName ) == 0 )
            return effectsList[loop]->effect;
    }

	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3D10_SHADER_DEBUG;
	dwShaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;;
#endif
	ID3D10Blob* compilationErrors = 0;

    ID3D10Effect*  fx;
    if(g_pd3dDevice)
    {

	    hr = D3DX10CreateEffectFromFile(	path, 
                                            NULL, 
                                            NULL, 
                                            "fx_4_0", 
                                            dwShaderFlags, 
                                            0,
										    g_pd3dDevice, 
                                            NULL, 
                                            NULL, 
                                            &fx, 
                                            &compilationErrors, 
                                            NULL );
    }
    else
    {
 		MessageBox( NULL,
			L"invalid device pointer EffectsManager::CreateEffect - bailing out!", L"Error", MB_OK );
        return NULL;
    }
	

    if(hr == D3D10_ERROR_FILE_NOT_FOUND)
    {
   
 		MessageBox( NULL,
			L"Can't find the fx file - bailing out!", L"Error", MB_OK );
        return NULL;
    }
    if(hr == E_FAIL){

		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			compilationErrors->Release();
		}
        return NULL;
    }
	if( FAILED( hr ) )
	{
		MessageBox( NULL,
			L"General Microsoft Bullshit error - bailing out!", L"Error", MB_OK );
        return NULL;
	}
    effectItem *effect = new effectItem();
    effect->effect = fx;
 

    if (bs)
    {
        wcscpy(effect->fxName, bs+1);
    }else{
        wcscpy(effect->fxName, path);
    }
    
    effectsList.push_back(effect);

    return fx;

}

ID3D10Effect* EffectsManager::getEffect( TCHAR *path )
{
    return NULL;
}

ID3D10InputLayout* EffectsManager::InitLayouts( ILOType iloType, effectPacketItem *epi)
{
 
	D3D10_PASS_DESC PassDesc;
    ID3D10InputLayout*      VertexLayout = NULL;
    HRESULT hr = S_OK;
    switch (iloType){
            case ILOType::P:
            {
	            //
	            // Position vertex
	            //
 	            D3D10_INPUT_ELEMENT_DESC posVertexDesc[] =
	            {
		            {"POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0}
	            };

	            //someFX->GetTechniqueByName("SOTech")->GetPassByIndex(0)->GetDesc(&PassDesc);
                //   HR(device->CreateInputLayout(posVertexDesc, sizeof( posVertexDesc ) / sizeof( D3D10_INPUT_ELEMENT_DESC ), PassDesc.pIAInputSignature,
	            //	PassDesc.IAInputSignatureSize, &g_pSOVertexLayout));
                break;
            }
        case PNT: // Position-normal-texture vertex
            {

	            D3D10_INPUT_ELEMENT_DESC posNormalTexVertexDesc[] =
	            {
		            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		            {"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0},
	            };
                //fx->GetTechniqueByName(ET)->GetPassByIndex(0)->GetDesc(&PassDesc);
                hr = g_pd3dDevice->CreateInputLayout(posNormalTexVertexDesc, sizeof( posNormalTexVertexDesc ) / sizeof( D3D10_INPUT_ELEMENT_DESC ), epi->vertexShader->shaderData->GetBufferPointer(),
	                                                 epi->vertexShader->shaderData->GetBufferSize(), &VertexLayout);
                break;
            }
        case PTNT: // Position-tangent-normal-texture vertex
            {
	            D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	            {
		            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		            {"TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		            {"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0},
		            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 36, D3D10_INPUT_PER_VERTEX_DATA, 0},
	            };

              //  someFX->GetTechniqueByName("SomeTech")->GetPassByIndex(0)->GetDesc(&PassDesc);
              //  HR(device->CreateInputLayout(vertexDesc, sizeof( vertexDesc ) / sizeof( D3D10_INPUT_ELEMENT_DESC ), PassDesc.pIAInputSignature,
		            //PassDesc.IAInputSignatureSize, &PosTangentNormalTex));
                break;
            }
        case PVSAT:// Particle vertex
            {
	            D3D10_INPUT_ELEMENT_DESC particleDesc[] =
	            {
		            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		            {"VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		            {"SIZE",     0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0},
		            {"AGE",      0, DXGI_FORMAT_R32_FLOAT,       0, 32, D3D10_INPUT_PER_VERTEX_DATA, 0},
		            {"TYPE",     0, DXGI_FORMAT_R32_UINT,        0, 36, D3D10_INPUT_PER_VERTEX_DATA, 0},
	            };

	            //someFX->GetTechniqueByName("SomeTech")->GetPassByIndex(0)->GetDesc(&PassDesc);
                //   HR(device->CreateInputLayout(particleDesc, sizeof( particleDesc ) / sizeof( D3D10_INPUT_ELEMENT_DESC ), PassDesc.pIAInputSignature,
	            //	PassDesc.IAInputSignatureSize, &Particle));

                break;
            }
        case PC:// Square vertex
            {
	            D3D10_INPUT_ELEMENT_DESC squareDesc[] =
	            {
                    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D10_INPUT_PER_VERTEX_DATA, 0},
                    {"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
                    //{"SIZE",     0, DXGI_FORMAT_R32G32_FLOAT,       0, 28, D3D10_INPUT_PER_VERTEX_DATA, 0},
	            };

	            //g_pEffect->GetTechniqueByName(pDrawTechniqueName)->GetPassByIndex(0)->GetDesc(&PassDesc);
                //   hr = g_pd3dDevice->CreateInputLayout(squareDesc, sizeof( squareDesc ) / sizeof( D3D10_INPUT_ELEMENT_DESC ), PassDesc.pIAInputSignature,
	            //	PassDesc.IAInputSignatureSize, &g_pVertexLayout);
                break;
            }
    }
    InputLayoutItem * ilo = new InputLayoutItem[1];
    ilo->IPLO = VertexLayout;
    //ilo->inputLayoutName = "PNT";
    inputLayoutsList.push_back(ilo);
    return VertexLayout;
}


//void BindAppShaderVars()
//{
//	g_pTreeTechnique           = g_pEffect->GetTechniqueByName("TreeTech");
//	g_SceneGraph->m_sceneGraph[0].pET = g_pTreeTechnique;
//	g_pCanopyTechnique           = g_pEffect->GetTechniqueByName("CanopyTech");
//	g_SceneGraph->m_sceneGraph[1].pET = g_pCanopyTechnique;
//	//g_pDrawTechnique              = g_pEffect->GetTechniqueByName( pDrawTechniqueName );
//	//g_pStreamOutTechnique         = g_pEffect->GetTechniqueByName( pStreamOutTechniqueName );
//	//g_pmatWorldVariable			  = g_pEffect->GetVariableByName( "gWorld" )->AsMatrix();
//	//g_pmatViewVariable			  = g_pEffect->GetVariableByName( "gView" )->AsMatrix();
//	//g_pmatProjectionVariable	  = g_pEffect->GetVariableByName( "gProj" )->AsMatrix();
//    g_pmatViewProjectionVariable  = g_pEffect->GetVariableByName( "gViewProj" )->AsMatrix();
//    g_pTexArrayVariable           = g_pEffect->GetVariableByName( "gTex" )->AsShaderResource();
//	g_pEyePosVar                  = g_pEffect->GetVariableByName( "gEyePosW")->AsVector();
//    //g_pLightDirVar                = g_pEffect->GetVariableByName( "gLightDirW")->AsVector();
//    g_pLightDir1Var                = g_pEffect->GetVariableByName( "gLightDir1W")->AsVector();
//    g_pLightDir2Var                = g_pEffect->GetVariableByName( "gLightDir2W")->AsVector();
//    g_pLightDir3Var                = g_pEffect->GetVariableByName( "gLightDir3W")->AsVector();
//	g_pSpecVariable				  = g_pEffect->GetVariableByName("gSpec")->AsVector();
//	g_pEl1Variable				  = g_pEffect->GetVariableByName("gEl1")->AsVector();
//	g_pEl2Variable				  = g_pEffect->GetVariableByName("gEl2")->AsVector();
//	g_pEl3Variable				  = g_pEffect->GetVariableByName("gEl3")->AsVector();
//    g_pPowVariable                = g_pEffect->GetVariableByName( "gmPow")->AsScalar();
//    //g_pFrameParam1                = g_pEffect->GetVariableByName( "gFrameParam1")->AsScalar();
//    //g_pFrameParam2                = g_pEffect->GetVariableByName( "gFrameParam2")->AsScalar();
//    //g_pQuadSize                   = g_pEffect->GetVariableByName( "gQuadSize")->AsScalar();
//}