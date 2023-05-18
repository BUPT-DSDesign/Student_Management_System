import { service } from '@/utils/request'

const IsCourseArrive = (curTime) => {
    const url = '/polling/is_course_arrive?cur_time=' + curTime
    return service({
        url: url,
        method: 'get',
    })
}
const IsEventArrive = (curTime) => {
    const url = '/polling/is_event_arrive?cur_time=' + curTime
    return service({
        url: url,
        method: 'get',
    })
}