from vtk import *
import itk

modelPath = "./BRATS_HG0015_T1C.mha"
maskPath = "./mask.mha"

###################################################################################################
def AddSlider(interactor, value_range, x, y, length=0.25, title="", default_value=None, callback=lambda x: _, integer_steps=False):
    assert 0 <= x <= 1 and 0 <= y <= 1

    def _cb(s, *args):
        slider_representation = s.GetSliderRepresentation()
        value = slider_representation.GetValue()
        if integer_steps: 
            value = round(value)
            slider_representation.SetValue(value)
        callback(value)

    # Set slider properties
    slider = vtkSliderRepresentation2D()
    slider.SetMinimumValue(value_range[0])
    slider.SetMaximumValue(value_range[-1])
    slider.SetValue(value_range[0] if default_value is None else default_value)
    slider.SetTitleText(title)
    slider.ShowSliderLabelOn()
    slider.SetSliderWidth(0.03)
    slider.SetSliderLength(0.0001)
    slider.SetEndCapWidth(0)
    slider.SetTitleHeight(0.02)
    slider.SetTubeWidth(0.005)
    
    # Set the slider position
    slider.GetPoint1Coordinate().SetCoordinateSystemToNormalizedDisplay();
    slider.GetPoint1Coordinate().SetValue(x, y);
    slider.GetPoint2Coordinate().SetCoordinateSystemToNormalizedDisplay();
    slider.GetPoint2Coordinate().SetValue(x + length, y);

    # Add the slider to the UI
    sliderWidget = vtkSliderWidget()
    sliderWidget.SetInteractor(interactor);
    sliderWidget.SetRepresentation(slider);
    sliderWidget.EnabledOn();
    
    # Add callback
    sliderWidget.AddObserver("InteractionEvent", _cb)
    
    return sliderWidget
    
def cb_opacity_brain(x):
    # Callback to update brain volume opacity
    fct_opacity_default.AddSegment(seg_min, 0., seg_max, x)
    
def cb_opacity_mask(x):
    # Callback to update mask opacity
    volume_mapper.SetMaskBlendFactor(x)

FloatImageType = itk.Image[itk.ctype("float"), 3]
reader = itk.ImageFileReader[FloatImageType].New()
reader.SetFileName(modelPath)


rescaled = itk.RescaleIntensityImageFilter.New(
    Input=reader.GetOutput(),
    OutputMinimum=0,
    OutputMaximum=255
)

binary_im = itk.ThresholdImageFilter.New(
    Input=rescaled,
    Lower=110,
)

GradientFilterType = itk.GradientMagnitudeRecursiveGaussianImageFilter[
    FloatImageType, FloatImageType
]
gradientMagnitude = GradientFilterType.New()
gradientMagnitude.SetSigma(1.0)
gradientMagnitude.SetInput(binary_im.GetOutput())
gradientMagnitude.Update()


watershed = itk.WatershedImageFilter.New(Input=gradientMagnitude.GetOutput())
watershed.SetThreshold(0.001)
watershed.SetLevel(0.15)
watershed.Update()

PixelType = itk.ctype("unsigned char")
RGBPixelType = itk.RGBPixel[PixelType]
RGBImageType = itk.Image[RGBPixelType, 3]

LabeledImageType = type(watershed.GetOutput())
ScalarToRGBColormapFilterType = itk.ScalarToRGBColormapImageFilter[
    LabeledImageType, RGBImageType
]

colormapImageFilter = ScalarToRGBColormapFilterType.New()
colormapImageFilter.SetColormap(
    itk.ScalarToRGBColormapImageFilterEnums.RGBColormapFilter_Hot
)
colormapImageFilter.SetInput(watershed.GetOutput())
colormapImageFilter.Update()

WriterType = itk.ImageFileWriter[RGBImageType]
writer = WriterType.New()
writer.SetFileName(maskPath)
writer.SetInput(colormapImageFilter.GetOutput())
writer.Update()

reader_brain = vtkMetaImageReader()
reader_brain.SetFileName(modelPath)
reader_brain.Update()
reader_mask = vtkMetaImageReader()
reader_mask.SetFileName(maskPath)
reader_mask.Update()

mapper = vtkOpenGLGPUVolumeRayCastMapper()

mapper.SetInputConnection(reader_brain.GetOutputPort())

volume = vtkVolume()
volume.SetMapper(mapper)

volume_property = volume.GetProperty()
volume_mapper = volume.GetMapper()

data_min, data_max = reader_brain.GetOutput().GetScalarRange()

seg_min, seg_max = 0, 0.6 * data_max
fct_color_default = vtkColorTransferFunction()
fct_color_default.AddRGBSegment(seg_min, *(0,0,0), seg_max, *(1,1,1))

fct_opacity_default = vtkPiecewiseFunction()
fct_opacity_default.AddSegment(seg_min, 0., seg_max, 0.1)

fct_color_mask = vtkColorTransferFunction()
fct_color_mask.AddRGBSegment(seg_min, *(0,0,0), seg_max, *(1,0,0))  

fct_opacity_mask = vtkPiecewiseFunction()
fct_opacity_mask.AddSegment(seg_min, 0., seg_max, 1.)

volume_property.SetColor(fct_color_default)
volume_property.SetScalarOpacity(fct_opacity_default)
volume_property.SetLabelColor(1, fct_color_mask)
volume_property.SetLabelScalarOpacity(1, fct_opacity_mask)

volume_mapper.SetMaskInput(reader_mask.GetOutput())

render = vtkRenderer()
render.AddVolume(volume)

renderWin = vtkRenderWindow()
renderWin.AddRenderer(render)

interactor = vtkRenderWindowInteractor()
interactor.SetRenderWindow(renderWin)

# Add all UI sliders
sl_0 = AddSlider(interactor=interactor, value_range=(0, 1), x=0.7, y=0.15, title="Brain Opacity", 
                 default_value=0.1, callback=cb_opacity_brain)
sl_1 = AddSlider(interactor=interactor, value_range=(0, 1), x=0.7, y=0.30, title="Tumor Highlight", 
                 default_value=0.7, callback=cb_opacity_mask)

interactor.Initialize()
renderWin.Render()
interactor.Start()
